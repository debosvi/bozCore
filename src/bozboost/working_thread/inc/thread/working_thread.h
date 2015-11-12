/*
 * Constantin Fishkin
 * 
 * constantin.fishkin@gmail.com
 *
 */

#ifndef QM_THREAD_THREAD_H
#define QM_THREAD_THREAD_H

#include <string>
#include <list>
#include <algorithm>
#include <functional>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mem_fn.hpp>

#include "detail/thread_os.h"

namespace qm {
namespace utils {


//! \class working_thread
//! \brief Thread wrapper with safe start/stop/pause/resume methods.
//! \details The class provide safe methods for thread control:
//! <code>start()</code>, <code>stop()</code>, <code>pause()</code> and <code>resume()</code>.
//! The methods are blocking: they will return only after the operation is completed.
//! \n The class is abstract. To use it a derived class must implement pure virtual method <code>action()</code>.
//! \n The <code>action()</code> must exit if the <code>is_interrupted()</code> flag is set to <code>true</code>.
//! That requires periodical flag checking in the <code>action()</code> implemenation code. The checking period defines
//! the waiting time of the <code>stop()</code>, <code>pause()</code> operations. Also the derived class can implement
//! the <code>on_interrupt()</code> virtual method. This callback is invoked each time either the <code>stop()</code> or the <code>pause()</code> 
//! method is called. The callback implementation should be used to force the <code>action()</code> method 
//! to check the <code>is_interrupted()</code> flag. For example:
//!\code
//!     virtual bool action()
//!     {
//!         boost::unique_lock<boost::mutex> lock(m_guard);
//!         while(!is_interrupted())
//!         {
//!             m_signal.wait(lock);
//!         }
//!         return true;
//!     }
//! 
//!     virtual void on_interrupt()
//!     {
//!         boost::unique_lock<boost::mutex> lock(m_guard);
//!         // in this point is_interrupted() is true
//!         assert(is_interrupted());
//!         m_signal.notify_one();
//!     }
//!  \endcode
//! \n The <code>stop()</code> (but not <code>pause()</code>) method can
//! use <code>force_interrupt</code> flag: <code>stop(true)</code>. This flag will interrupt the <code>action()</code> waiting at any
//! <i>boost thread interruption point</i> (for example, <code>boost::condition_variable::wait()</code>).
//! \n The <code>action()</code> will be called again if it returns true. If it returns <code>false</code> the thread
//! execution will be completed.
//! \n In the paused state the thread will sleep and will not call <code>action()</code> till the execution is resumed. 
//! This paused state can be used for re-configuration of the working thread data. Without such paused state the
//! <code>action()</code> must use locks to protect sensitive data from simultaneous accees by multiple threads.
//! This approach allows the thread to avoid using locks in run time.
//! \n The class's virtual <i>dtor</i> will assert if thread state is not <code>completed</code>. To prevent the assertion
//! <i>dtor</i> of the derived class should call the <code>stop()</code> method. 
//! \n The derived class also can implement optional virtual methods <code>on_start()</code> and <code>on_exit()</code>.
//! \n An auxiliary <code>bulk</code> class performs the blocking <code>start()</code>, <code>stop()</code>, 
//! <code>pause()</code> and <code>resume()</code> operations simultaneously for several thread objects.
class working_thread
{

friend class bulk;

public:
    typedef thread_id_t id_type;
    typedef handle_t handle_type;

    
    /* possible transitions
    +------+----------+----------+----------+----------+
    |      |init      |running   |paused    |completed |
    +----- +----------+----------+----------+----------+
    |stop  |completed |completed |completed |completed |
    |      |true      |true      |true      |true      |
    +------+----------+----------+----------+----------+
    |pause |init      |paused    |paused    |completed |
    |      |false     |true      |true      |false     |
    +------+----------+----------+----------+----------+
    |resume|init      |running   |running   |completed |
    |      |false     |true      |true      |false     |
    +-----------------+----------+----------+----------+
    |start |running   |running   |paused    |completed |
    |      |true      |true      |false     |false     |
    +------+----------+----------+----------+----------+
    */
    //! thread states
    enum state_type
    {
        //! thread did not start yet
        init,
        //! thread is paused
        paused,     
        //! thread is running
        running,    
        //! thread function is completed
        completed   
    };
    
    static const size_t DEFAULT_STACK_VALUE;
    static const id_type INVALID_ID_VALUE;
            
    typedef priority_data priority_type;
    typedef boost::system::error_code error_type;

    //! \class bulk
    //! \brief An auxiliary class that performs the blocking <code>start()</code>, <code>stop()</code>, 
    //! <code>pause()</code> and <code>resume()</code> operations simultaneously for several thread objects.
    //! \details The class accesses the hidden non-blocking methods of the <code>working_thread</code> class 
    //! and performs the <code>start()</code>, <code>stop()</code>, 
    //! <code>pause()</code> and <code>resume()</code> operations simultaneously for several thread objects.
    //! The class methods will be completed when the corresponding operation is completed for every thread object 
    //! in the bulk is complited. This approach can significantly reduce operation time for a thread pool.
    struct bulk
    {
        //! default ctor
        bulk(){}

        //! ctor accepts one thread object
        //! and adds it to the inner bulk list
        bulk(working_thread* obj)
        {
            m_objects.push_back(obj);
        }

        //! ctor accepts two thread objects
        //! and adds them to the inner bulk list
        bulk(working_thread* obj1, working_thread* obj2)
        {
            m_objects.push_back(obj1);
            m_objects.push_back(obj2);
        }

        //! ctor accepts three thread objects
        //! and adds them to the inner bulk list
        bulk(working_thread* obj1, working_thread* obj2, 
            working_thread* obj3)
        {
            m_objects.push_back(obj1);
            m_objects.push_back(obj2);
            m_objects.push_back(obj3);
        }

        //! ctor accepts four thread objects
        //! and adds them to the inner bulk list
        bulk(working_thread* obj1, working_thread* obj2, 
            working_thread* obj3, working_thread* obj4)
        {
            m_objects.push_back(obj1);
            m_objects.push_back(obj2);
            m_objects.push_back(obj3);
            m_objects.push_back(obj4);
        }

        //! the operator adds the thread object
        //! to the inner bulk list
        void operator+=(working_thread* obj)
        {
            m_objects.push_back(obj);
        }
               
        //! counts amount of thread objects in an external containter
        //! that are in the given state
        //! \param begin the first iterator of the input container.
        //! \param end the last iterator of the input container.
        //! \param state state to count
        //! \return amount of thread objects in the external containter
        //! that are in the given state
        template<typename It>
        size_t count(It begin, It end, working_thread::state_type state)
        {
            size_t rc=0;
            for (; begin != end; ++begin) 
                if(get_ptr(*begin)->state()==state) ++rc;

            return rc;
        }
        //! perform start operation on an external container
        //! of the thread objects. The method is blocking.
        //! The container can be a container of objects, pointers
        //! or boost's shared pointers.
        //! \param begin the first iterator of the input container.
        //! \param end the last iterator of the input container.
        template<typename It>
        static void start(It begin, It end)
        {
            std::list<working_thread*> waiting_list;
            for (; begin != end; ++begin) 
                launch_item(get_ptr(*begin), waiting_list);
            
            std::for_each(waiting_list.begin(), waiting_list.end(), 
                boost::mem_fn(&working_thread::wait_till_launched));
        }

        //! perform stop operation on an external container
        //! of the thread objects. The method will block till
        //! all objects in the container are in the <code>completed</code> state.
        //! The container can be a container objects, pointers
        //! or boost's shared pointers.
        //! \param begin the first iterator of the input container.
        //! \param end the last iterator of the input container.
        //! \param force_interrupt forces using the boost's inerruption points:
        //! if the thread in waitng at any of the <i>boost's interruption points</i>, it
        //! will be interrupted as well.
        template<typename It>
        static void stop(It begin, It end, bool force_interrupt=false)
        {
            for (It item=begin; item != end; ++item) 
                get_ptr(*item)->stop_request(force_interrupt);

            for (It item=begin; item != end; ++item) 
                get_ptr(*item)->join();
        }

        //! perform pause operation on an external container
        //! of the thread objects. The method is blocking.
        //! The container can be a container of objects, pointers
        //! or boost's shared pointers.
        //! \param begin the first iterator of the input container.
        //! \param end the last iterator of the input container.
        template<typename It>
        static void pause(It begin, It end)
        {
            std::list<working_thread*> waiting_list;
            for (; begin != end; ++begin) 
                pause_item(get_ptr(*begin), waiting_list);
            
            std::for_each(waiting_list.begin(), waiting_list.end(), 
                boost::mem_fn(&working_thread::wait_till_paused));
        }

        //! perform resume operation on an external container
        //! of the thread objects. The method is blocking.
        //! The container can be a container of objects, pointers
        //! or boost's shared pointers.
        //! \param begin the first iterator of the input container.
        //! \param end the last iterator of the input container.
        template<typename It>
        static void resume(It begin, It end)
        {
            std::list<working_thread*> waiting_list;
            for (; begin != end; ++begin) 
                resume_item(get_ptr(*begin), waiting_list);
            
            std::for_each(waiting_list.begin(), waiting_list.end(), 
                boost::mem_fn(&working_thread::wait_till_resumed));
        }
         
        //! counts amount of thread objects in the inner containter
        //! that are in the given state
        //! \param state state to count
        //! \return amount of thread objects in the inner containter
        //! that are in the given state
        size_t count(working_thread::state_type state)
        {
            return count(m_objects.begin(), m_objects.end(),state);
        }

        //! perform start operation on the inner thread object container.
        //! The method is blocking.
        //! \sa <code>operator+=(working_thread* obj)</code>.
        void start()
        {
            return start(m_objects.begin(), m_objects.end());
        }

        //! perform stop operation on the inner thread object container.
        //! The method will block till all objects in the container 
        //! are in the <code>completed</code> state.
        //! \param force_interrupt forces using the boost's inerruption points:
        //! if the thread in waitng at any of the boost's inerruption points, it
        //! will be interrupted as well.
        void stop(bool force_interrupt=false)
        {
            stop(m_objects.begin(), m_objects.end(), force_interrupt);
        }

        //! perform pause operation on the inner thread object container.
        //! The method is blocking.
        //! \sa <code>operator+=(working_thread* obj)</code>.
        void pause()
        {
            return pause(m_objects.begin(), m_objects.end());
        }

        //! perform resume operation on the inner thread object container.
        //! The method is blocking.
        //! \sa <code>operator+=(working_thread* obj)</code>.
        void resume()
        {
            return resume(m_objects.begin(), m_objects.end());
        }
        
    private:
        
        static working_thread* get_ptr(boost::shared_ptr<working_thread> obj)
        {
            return obj.get();
        }
                
        static working_thread* get_ptr(working_thread* obj)
        {
            return obj;
        }
                
        static working_thread* get_ptr(working_thread& obj)
        {
            return &obj;
        }
        
        static void launch_item(working_thread* obj, 
            std::list<working_thread*>& ls)
        {
            event_status rc=obj->start_event();
            if(rc.wait) ls.push_back(obj);
        }

        static void pause_item(working_thread* obj, 
            std::list<working_thread*>& ls)
        {
            event_status rc=obj->pause_event();
            if(rc.wait) ls.push_back(obj);
        }

        static void resume_item(working_thread* obj, 
            std::list<working_thread*>& ls)
        {
            event_status rc=obj->resume_event();
            if(rc.wait) ls.push_back(obj);
        }
                
    private:
        std::list<working_thread*> m_objects;
    };
        
public:
    //! ctor accepts name, thread priority and stack size
    //! \param name thread name. Default name is the empty string.
    //! \param p thread priority. Dafault value is the system default.
    //! \param stack_size thread stack size. Dafault value is the system default.
    working_thread
    (
        const std::string& name=std::string(),
        priority_type p=priority_type(), 
        size_t stack_size=DEFAULT_STACK_VALUE
    ) :
        m_name(name),
        m_priority(p),
        m_stack_size(stack_size),
        m_state(init),
        m_request(rq_none),
        m_id(INVALID_THREAD_ID)
    {}

    virtual ~working_thread()
    {
        // stop() must be called before
        // the derived class' dtor is completed.
        // stop() cannot be called here
        // because the thread's main function calls to
        // class's virtual functions including the pure
        // virtual action()
        BOOST_ASSERT_MSG(detached(), 
            "The thread function must be completed at this point");
    }

    //! gets the thread name
    const std::string& name() const
    {
        return m_name;
    }

    //! gets the thread priority
    priority_type priority() const
    {
        return m_priority;
    }

    //! gets the thread stack size
    size_t stack_size() const
    {
        return m_stack_size;
    }

    //! starts the thread. The method is blocking.
    bool start();

    //! stops the thread. The method is blocking.
    //! \param force_interrupt forces using the boost's inerruption points:
    //! if the thread in waitng at any of the boost's inerruption points, it
    //! will be interrupted as well.
    void stop(bool force_interrupt=false);

    //! pauses the thread. The method is blocking.
    bool pause();

    //! resumes the thread. The method is blocking.
    bool resume();

    //! waits till the thread is completed. The method is blocking.
    void join();

    //! gets the platform specific thread id
    id_type id() const;

    //! gets the platform specific thread handle
    handle_type handle();
    
    //! gets the thread current state
    state_type state() const
    {
        return m_state;
    }

    //! true if the thread is completed or not started yet
    bool detached() const
    {
        return init==m_state || completed==m_state;
    }

    //! true if the thread is running or paused
    bool attached() const
    {
        return !detached();
    }

    //! returns last error
    const error_type& last_error() const
    {
        return m_error;
    }
    
protected:

    //! the action callback implemented in the derived class.
    //! The callback will be called in the context of the
    //! running thread again and again till it returns true.
    //! To terminate the thread the callback should return false.
    //! The callback <b>must</b> exit if the <code>is_interrupted()</code> 
    //! flag is set to <code>true</code>.
    //! The method is pure virtual and must be implemented in derived classes.
    //! \return code>true</code> to continue the execution and
    //! code>false</code> to complete the thread function.
    virtual bool action()=0;

    //! the callback is called after the thread started
    //! its execution. The thread state is <code>running</code>.
    //! The method is called in context of the running thread.
    //! The method can be implemented in derived classes.
    virtual void on_start(){}

    //! the callback is called after the thread ceased to call
    //! the <code>action</code> method but
    //! before the thread function is completed.
    //! The method is called in context of the running thread.
    //! The thread state is <code>completed</code>.
    //! The method can be implemented in derived classes.
    virtual void on_exit(){}
    
    //! the callback is invoked each time either the <code>stop()</code>
    //! or <code>pause()</code> is called. A derived class can use this
    //! callback to force the <code>action()</code> to check 
    //! the<code>is_interrupted()</code> flag.
    virtual void on_interrupt(){}

    //! signals to the <code>action()</code> to exit.
    //! The flag is set on either stop or pause request.
    bool is_interrupted() const
    {
        return rq_none!=m_request;
    }
private:
    void main();
    void idle();
    void signal_state(state_type state);
    enum erequest{rq_none, rq_pause, rq_stop};

    void request(erequest rq)
    {
        m_request=rq;
    }

    void stop_request(bool force);
    bool launch();
    void wake_up(erequest rq);
    
    bool wait_till_launched();
    bool wait_till_resumed();
    bool wait_till_paused();

    struct event_status
    {
        event_status() : 
            success(false),
            wait(false)
        {}

        event_status(bool success_, bool wait_) : 
            success(success_),
            wait(wait_)
        {}

        event_status(bool rc) : 
            success(rc),
            wait(rc)
        {}

        bool success;
        bool wait;
    };

    event_status start_event();
    event_status pause_event();
    event_status resume_event();
private:
    const std::string m_name;
    const priority_type m_priority;
    const size_t m_stack_size;
    volatile state_type m_state;
    volatile erequest m_request;
    volatile id_type m_id;
    boost::thread m_thread;
    boost::system::error_code m_error;
    boost::mutex m_guard;
    boost::condition_variable m_signal;
    boost::condition_variable m_pause;
};

}
}

#endif
