/*!
 * \file bulk_thread.hpp
 * \brief Bulk Thread interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#ifndef _BOZ_THREAD_BULK_H_
#define _BOZ_THREAD_BULK_H_

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#include "thread/working_thread.hpp"

namespace boz {
namespace thread {

//! \class bulk
//! \brief An auxiliary class that performs the blocking <code>start()</code>, <code>stop()</code>, 
//! <code>pause()</code> and <code>resume()</code> operations simultaneously for several thread objects.
//! \details The class accesses the hidden non-blocking methods of the <code>working_thread</code> class 
//! and performs the <code>start()</code>, <code>stop()</code>, 
//! <code>pause()</code> and <code>resume()</code> operations simultaneously for several thread objects.
//! The class methods will be completed when the corresponding operation is completed for every thread object 
//! in the bulk is complited. This approach can significantly reduce operation time for a thread pool.
class bulk {
public:
    //! default ctor
    explicit bulk();

    //! dtor
    ~bulk();

    //! ctor accepts one thread object
    //! and adds it to the inner bulk list
    bulk(working_thread* obj);

    //! ctor accepts two thread objects
    //! and adds them to the inner bulk list
    bulk(working_thread* obj1, working_thread* obj2);

    //! ctor accepts three thread objects
    //! and adds them to the inner bulk list
    bulk(working_thread* obj1, working_thread* obj2, working_thread* obj3);

    //! ctor accepts four thread objects
    //! and adds them to the inner bulk list
    bulk(working_thread* obj1, working_thread* obj2, working_thread* obj3, working_thread* obj4);

    //! the operator adds the thread object
    //! to the inner bulk list
    void operator+=(working_thread* obj);
           
    //! counts amount of thread objects in an external containter
    //! that are in the given state
    //! \param begin the first iterator of the input container.
    //! \param end the last iterator of the input container.
    //! \param state state to count
    //! \return amount of thread objects in the external containter
    //! that are in the given state
    template<typename It>
    size_t count(It begin, It end, working_thread::state_type state);
    
    //! perform start operation on an external container
    //! of the thread objects. The method is blocking.
    //! The container can be a container of objects, pointers
    //! or boost's shared pointers.
    //! \param begin the first iterator of the input container.
    //! \param end the last iterator of the input container.
    template<typename It>
    static void deletion(It begin, It end);
    
    //! perform start operation on an external container
    //! of the thread objects. The method is blocking.
    //! The container can be a container of objects, pointers
    //! or boost's shared pointers.
    //! \param begin the first iterator of the input container.
    //! \param end the last iterator of the input container.
    template<typename It>
    static void start(It begin, It end);
    
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
    static void stop(It begin, It end, bool force_interrupt=false);

    //! perform pause operation on an external container
    //! of the thread objects. The method is blocking.
    //! The container can be a container of objects, pointers
    //! or boost's shared pointers.
    //! \param begin the first iterator of the input container.
    //! \param end the last iterator of the input container.
    template<typename It>
    static void pause(It begin, It end);

    //! perform resume operation on an external container
    //! of the thread objects. The method is blocking.
    //! The container can be a container of objects, pointers
    //! or boost's shared pointers.
    //! \param begin the first iterator of the input container.
    //! \param end the last iterator of the input container.
    template<typename It>
    static void resume(It begin, It end);
     
    //! counts amount of thread objects in the inner containter
    //! that are in the given state
    //! \param state state to count
    //! \return amount of thread objects in the inner containter
    //! that are in the given state
    size_t count(working_thread::state_type state);

    //! perform deletion operation on the inner thread object container.
    //! The method is blocking.
    //! \sa <code>operator+=(working_thread* obj)</code>.
    void deletion();

    //! perform start operation on the inner thread object container.
    //! The method is blocking.
    //! \sa <code>operator+=(working_thread* obj)</code>.
    void start();

    //! perform stop operation on the inner thread object container.
    //! The method will block till all objects in the container 
    //! are in the <code>completed</code> state.
    //! \param force_interrupt forces using the boost's inerruption points:
    //! if the thread in waitng at any of the boost's inerruption points, it
    //! will be interrupted as well.
    void stop(bool force_interrupt=false);

    //! perform pause operation on the inner thread object container.
    //! The method is blocking.
    //! \sa <code>operator+=(working_thread* obj)</code>.
    void pause();

    //! perform resume operation on the inner thread object container.
    //! The method is blocking.
    //! \sa <code>operator+=(working_thread* obj)</code>.
    void resume();
    
private:
    
    static working_thread* get_ptr(boost::shared_ptr<working_thread> obj);
            
    static working_thread* get_ptr(working_thread* obj);
            
    static working_thread* get_ptr(working_thread& obj);
    
    static void launch_item(working_thread* obj, std::list<working_thread*>& ls);

    static void pause_item(working_thread* obj, std::list<working_thread*>& ls);

    static void resume_item(working_thread* obj, std::list<working_thread*>& ls);
            
private:
    std::list<working_thread*> m_objects;
};
        

} // namespace thread
} // namespace boz

#endif // _BOZ_THREAD_BULK_H_
