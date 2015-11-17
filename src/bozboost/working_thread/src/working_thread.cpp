/*
 * Constantin Fishkin
 * 
 * constantin.fishkin@gmail.com
 *
 */

#include "thread/working_thread.hpp"
#include "priv/event_status.hpp"

namespace boz {
namespace thread {

const size_t working_thread::DEFAULT_STACK_VALUE=0;
const working_thread::id_type working_thread::INVALID_ID_VALUE=INVALID_THREAD_ID;

working_thread::working_thread(const std::string& name, priority_type p, size_t stack_size) :
    m_name(name), m_priority(p), m_stack_size(stack_size), m_state(init), m_request(rq_none), m_id(INVALID_THREAD_ID) {
}
   
working_thread::~working_thread() {
    // stop() must be called before
    // the derived class' dtor is completed.
    // stop() cannot be called here
    // because the thread's main function calls to
    // class's virtual functions including the pure
    // virtual action()
    BOOST_ASSERT_MSG(detached(), "The thread function must be completed at this point");
}   

const std::string& working_thread::name() const {
    return m_name;
}

//! gets the thread priority
working_thread::priority_type working_thread::priority() const {
    return m_priority;
}

//! gets the thread stack size
size_t working_thread::stack_size() const {
    return m_stack_size;
}

working_thread::state_type working_thread::state() const {
    return m_state;
}

bool working_thread::detached() const {
    return init==m_state || completed==m_state;
}

//! true if the thread is running or paused
bool working_thread::attached() const {
    return !detached();
}

//! returns last error
const working_thread::error_type& working_thread::last_error() const {
    return m_error;
}
    
event_status working_thread::start_event()
{
    // no re-run
    if(completed==m_state || paused==m_state) 
        return event_status(false);
    
    // already runs
    if(running==m_state) 
        return event_status(true, false);
    
    // try to launch
    // the call may fail due to a system error
    return event_status(launch());
}

bool working_thread::start() {
    event_status rc=start_event();

    if(rc.wait) 
        rc.success&=wait_till_launched();
    
    return rc.success;
}

void working_thread::stop(bool force_interrupt) {
    stop_request(force_interrupt);
        
    join();
}

void working_thread::stop_request(bool force) {
    // do nothingfor completed state
    if(completed==m_state) 
        return;

    // if the thread is not launched
    // just change the state
    if(init==m_state) 
    {
        m_state=completed;
        return;
    }

    wake_up(rq_stop);

    // callback is called
    // after the flag is changed
    on_interrupt();
    
    // interrupt waiting in 
    // any interruption point
    if(force) m_thread.interrupt();
}

event_status working_thread::pause_event() {
    // already paused
    if(paused==m_state) 
        return event_status(true, false);
    
    // cannot pause detached state
    if(detached()) 
        return event_status(false);
    
    request(rq_pause);

    // callback is called
    // after the flag is changed
    on_interrupt();

    return event_status(true);
}

bool working_thread::pause() {
    event_status rc=pause_event();

    if(rc.wait) 
        rc.success&=wait_till_paused();
    
    return rc.success;
}

event_status working_thread::resume_event() {
    // already resumed
    if(running==m_state) 
        return event_status(true, false);
    
    // cannot resume detached state
    if(detached()) 
        return event_status(false);
    
    wake_up(rq_none);

    return event_status(true);
}

bool working_thread::resume() {
    event_status rc=resume_event();

    if(rc.wait) 
        rc.success&=wait_till_resumed();
    
    return rc.success;
}

void working_thread::join() {
    // can throws boost::thread_interrupted 
    // to prevent it we must disable the interruption
    // and in any case we do not want join() complete
    // before the thread
    boost::this_thread::disable_interruption di;
    m_thread.join();
}

working_thread::id_type working_thread::id() const {
    return m_id;
}

working_thread::handle_type working_thread::handle() {
    // only non constant method version because
    // with the native handle the thread can be
    // alternated "behind the curtain"

    // in detached states (init/completed) handle is not defined
    return detached()?NULL:(void*)m_thread.native_handle();
}

void working_thread::wake_up(erequest rq) {
    // signal that the pause is over
    boost::unique_lock<boost::mutex> lock(m_guard);
    m_request=rq;

    m_pause.notify_one();
}

bool working_thread::launch() {
    boost::thread_attributes attr;
    // set stack size - crossplatform
    attr.set_stack_size(m_stack_size);
    // set thread priority - platform specific
    unsigned rc=set_attribute_priority(m_priority.priority_value(), 
        m_priority.priority_class(), attr.native_handle());
    if(rc) m_error.assign(rc, boost::system::system_category());
    // launch void main() member function
    // in separated thread using attriutes

    // can throw boost::thread_resource_error
    try
    {
        m_thread=boost::thread(attr, boost::bind(&working_thread::main, this));
    }
    catch(boost::thread_resource_error& err)
    {
        m_error=err.code();
        return false;
    }

    return true;
}

bool working_thread::wait_till_launched() {
    // wait till the thread is launched
    {    
        boost::unique_lock<boost::mutex> lock(m_guard);
        while(running!=m_state && completed!=m_state)
        {
            m_signal.wait(lock);
        }
    }
     
    if(running!=m_state) return false;

    // set priority for already running thread
    // if it is needed
    unsigned rc=adjust_priority(m_priority.priority_value(), 
        m_priority.priority_class(), handle());

    if(rc) m_error.assign(rc, boost::system::system_category());
    return true;
}

bool working_thread::wait_till_resumed() {
    // wait till the thread is resumed
    {    
        boost::unique_lock<boost::mutex> lock(m_guard);

        while(running!=m_state && completed!=m_state)
        {
            m_signal.wait(lock);
        }
    }

    return running==m_state;
}

bool working_thread::wait_till_paused() {
    // wait till the thread is paused
    {    
        boost::unique_lock<boost::mutex> lock(m_guard);
        while(paused!=m_state && completed!=m_state)
        {
            m_signal.wait(lock);
        }
    }

    return paused==m_state;
}

void working_thread::idle() {
    // signal paused state
    signal_state(paused);
    
    // wait in the paused state
    {    
        boost::unique_lock<boost::mutex> lock(m_guard);

        while(rq_pause==m_request)
        {
            m_pause.wait(lock);
        }
    }

    // signal running state
    signal_state(running);
}

void working_thread::main() {
    // read and store current thread id
    m_id=get_current_thread_id();
    // signal that the thread is running
    signal_state(running);
    // perform on-start custom action
    on_start();

    // can throw const boost::thread_interrupted
    // if interrupt() was call in any interrupt
    // point
    try
    {
        while(rq_stop!=m_request)
        {
            while(rq_none==m_request)
            {
                if(!action()) break;
            }

            if(rq_pause!=m_request) break;

            idle();
        }
    }
    catch(const boost::thread_interrupted&)
    {
    }
        
    // update state
    signal_state(completed);
    // perform on-exit custom action
    // after the state was updated
    on_exit();
    // clear id
    m_id=INVALID_THREAD_ID;
}

void working_thread::signal_state(state_type state) {
    // update the state
    // and signal that the thread is 
    // in new state 
    boost::unique_lock<boost::mutex> lock(m_guard);
    m_state=state;
    
    m_signal.notify_one();
}

void working_thread::on_start() {
}

void working_thread::on_exit() {
}

void working_thread::on_interrupt() {
}

bool working_thread::is_interrupted() const {
    return rq_none!=m_request;
}

void working_thread::request(erequest rq) {
    m_request=rq;
}

} // namespace thread
} // namespace boz
