/*!
 * \file bulk_thread.cpp
 * \brief Bulk Thread implementation.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#include <boost/mem_fn.hpp>

#include "thread/bulk_thread.hpp"
#include "priv/event_status.hpp"

namespace boz {
namespace thread {

bulk::bulk() {    
}

bulk::~bulk() { 
    deletion();
}

bulk::bulk(working_thread* obj) {
    m_objects.push_back(obj);
}

bulk::bulk(working_thread* obj1, working_thread* obj2) {
    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
}

bulk::bulk(working_thread* obj1, working_thread* obj2, working_thread* obj3) {
    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
    m_objects.push_back(obj3);
}

bulk::bulk(working_thread* obj1, working_thread* obj2, working_thread* obj3, working_thread* obj4) {
    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
    m_objects.push_back(obj3);
    m_objects.push_back(obj4);
}

void bulk::operator+=(working_thread* obj) {
    m_objects.push_back(obj);
}
           
template<typename It>
size_t bulk::count(It begin, It end, working_thread::state_type state) {
    size_t rc=0;
    for (; begin != end; ++begin) 
        if(get_ptr(*begin)->state()==state) ++rc;

    return rc;
}

template<typename It>
void bulk::deletion(It begin, It end) {
    for (; begin != end; ++begin)
        delete get_ptr(*begin);    
}

template<typename It>
void bulk::start(It begin, It end) {
    std::list<working_thread*> waiting_list;
    for (; begin != end; ++begin) 
        launch_item(get_ptr(*begin), waiting_list);
    
    std::for_each(waiting_list.begin(), waiting_list.end(), 
        boost::mem_fn(&working_thread::wait_till_launched));
}

template<typename It>
void bulk::stop(It begin, It end, bool force_interrupt) {
    for (It item=begin; item != end; ++item) 
        get_ptr(*item)->stop_request(force_interrupt);

    for (It item=begin; item != end; ++item) 
        get_ptr(*item)->join();
}

template<typename It>
void bulk::pause(It begin, It end) {
    std::list<working_thread*> waiting_list;
    for (; begin != end; ++begin) 
        pause_item(get_ptr(*begin), waiting_list);
    
    std::for_each(waiting_list.begin(), waiting_list.end(), 
        boost::mem_fn(&working_thread::wait_till_paused));
}

template<typename It>
void bulk::resume(It begin, It end) {
    std::list<working_thread*> waiting_list;
    for (; begin != end; ++begin) 
        resume_item(get_ptr(*begin), waiting_list);
    
    std::for_each(waiting_list.begin(), waiting_list.end(), 
        boost::mem_fn(&working_thread::wait_till_resumed));
}
 
size_t bulk::count(working_thread::state_type state) {
    return count(m_objects.begin(), m_objects.end(),state);
}

void bulk::deletion() {
    return deletion(m_objects.begin(), m_objects.end());
}

void bulk::start() {
    return start(m_objects.begin(), m_objects.end());
}

void bulk::stop(bool force_interrupt) {
    stop(m_objects.begin(), m_objects.end(), force_interrupt);
}

void bulk::pause() {
    return pause(m_objects.begin(), m_objects.end());
}

void bulk::resume() {
    return resume(m_objects.begin(), m_objects.end());
}

working_thread* bulk::get_ptr(boost::shared_ptr<working_thread> obj) {
    return obj.get();
}
        
working_thread* bulk::get_ptr(working_thread* obj) {
    return obj;
}
        
working_thread* bulk::get_ptr(working_thread& obj) {
    return &obj;
}

void bulk::launch_item(working_thread* obj, std::list<working_thread*>& ls) {
    event_status rc=obj->start_event();
    if(rc.wait) ls.push_back(obj);
}

void bulk::pause_item(working_thread* obj, std::list<working_thread*>& ls) {
    event_status rc=obj->pause_event();
    if(rc.wait) ls.push_back(obj);
}

void bulk::resume_item(working_thread* obj, std::list<working_thread*>& ls) {
    event_status rc=obj->resume_event();
    if(rc.wait) ls.push_back(obj);
}
        
        

} // namespace thread
} // namespace boz

