/*!
 * \file thread_os.cpp
 * \brief Working Thread OS dependent implementation.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#include "thread/detail/thread_os.hpp"

namespace boz {
namespace thread {

#if defined(BOOST_THREAD_PLATFORM_WIN32)
    // windows version
    #include "thread_win.cpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
    // pthread version
    #include "thread_lin.cpp"
#else
    #error "Threads are unavailable on this platform"
#endif

priority_data::priority_data() : 
    m_value(PRIORITY_DEFAULT_VALUE), m_class(PRIORITY_DEFAULT_VALUE) {
}

priority_data::priority_data(int pr) : 
    m_value(pr), m_class(PRIORITY_DEFAULT_VALUE) {
}

priority_data::priority_data(int pr, int cl) : 
    m_value(pr), m_class(cl) {
}
    
priority_data::priority_data(epriority ep) : 
    m_value(get_priority_value(ep)), m_class(get_priority_class(ep)) {
}

int priority_data::priority_value() const {
    return m_value;
}

int priority_data::priority_class() const {
    return m_class;
}

} // namespace thread
} // namespace boz

