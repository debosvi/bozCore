/*!
 * \file thread_os.hpp
 * \brief Working Thread OS dependent interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#ifndef _BOZ_THREAD_DETAIL_OS_HPP_
#define _BOZ_THREAD_DETAIL_OS_HPP_

#include <boost/thread.hpp>

namespace boz {
namespace thread {

typedef unsigned int thread_id_t;
const thread_id_t INVALID_THREAD_ID=0;

typedef void* handle_t;
const handle_t INVALID_THREAD_HANDLE=0;

//! \class priority_data
//! \brief Thread priority wrapper
//! \details The class work with both the platform independent enumerated priority values and
//! the platform specific values of priority and priority class.
//! It translates the platform independent enumerated priority values
//! into platform specific values of priority and priority class using platform defined functions. 
//! \n Meaning of the priority class also
//! depends on the platform: for example, for Linux it is scheduling type and for Windows - thread
//! priority class.
class priority_data
{
public:
    
    //! platform independent enumerated priority values
    enum epriority
    {
        //! Normal thread priority
        NORMAL=0,
        //! Thread prioity above the normal
        ABOVE,
        //! Thread prioity below the normal
        BELOW,
        //! High thread prioity
        HIGH,
        //! Low thread prioity
        LOW,
        //! Real time thread prioity
        RT,
        //! Idle process thread prioity
        IDLE
    };
    
    // differnt from any priority value
    enum { PRIORITY_DEFAULT_VALUE=999999 };

    //! default ctor
    //! uses the default values for both priority and class
    priority_data() : 
        m_value(PRIORITY_DEFAULT_VALUE), 
        m_class(PRIORITY_DEFAULT_VALUE) 
    {}

    //! ctor gets numerical platform specific
    //! priority value and uses the default
    //! priority class
    //! \param pr numerical platform specific
    //! priority value
    priority_data(int pr) : 
        m_value(pr), m_class(PRIORITY_DEFAULT_VALUE) 
    {}

    //! ctor gets numerical platform specific
    //! values for priority and priority class
    //! \param pr numerical platform specific
    //! priority value
    //! \param cl numerical platform specific
    //! priority class value
    priority_data(int pr, int cl) : 
        m_value(pr), m_class(cl) 
    {}

    //! ctor gets enumerated platform independent
    //! priority value and converts it to platform
    //! specific values
    priority_data(epriority ep);

    //! gets the platform specific priority value
    int priority_value() const {return m_value;}
    //! gets the platform specific priority class value
    int priority_class() const {return m_class;}
private:
    const int m_value;
    const int m_class;
};

#if defined(BOOST_THREAD_PLATFORM_WIN32)
    // windows version
    #include "thread_win.hpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
    // pthread version
    #include "thread_lin.hpp"
#else
    #error "Threads are unavailable on this platform"
#endif

//! returns current thread id
inline thread_id_t get_current_thread_id()
{
    return os_get_current_thread_id();
}

//! converts enumerated value into platform specific
//! thread priority value
inline int get_priority_value(priority_data::epriority ep)
{
    return os_get_priority_value(ep);
}

//! converts enumerated value into platform specific
//! thread priority class value
inline int get_priority_class(priority_data::epriority ep)
{
    return os_get_priority_class(ep);
}

inline priority_data::priority_data(epriority ep) : 
    m_value(get_priority_value(ep)),
    m_class(get_priority_class(ep))
{}

//! sets priority of the current thread
//! \param priority_value platform specific priority value
//! \param priority_class platform specific priority class value
//! \param h platform specific thread handle
//! \return 0 on success and system error code on failure
inline unsigned adjust_priority(int priority_value, int priority_class, handle_t h)
{
    return os_adjust_priority(priority_value, priority_class, h);
}

//! sets priority to thread attribute
//! \param priority_value platform specific priority value
//! \param priority_class platform specific priority class value
//! \param h platform specific thread attribute handle
//! \return 0 on success and system error code on failure
inline unsigned set_attrubute_priority(int priority_value, int priority_class, handle_t h)
{
    return os_set_attrubute_priority(priority_value, priority_class, h);
}

} // namespace thread
} // namespace boz

#endif // _BOZ_THREAD_DETAIL_OS_HPP_
