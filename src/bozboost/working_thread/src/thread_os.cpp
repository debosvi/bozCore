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
    #include "thread/detail/thread_win.cpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
    // pthread version
    #include "thread/detail/thread_lin.cpp"
#else
    #error "Threads are unavailable on this platform"
#endif

} // namespace thread
} // namespace boz

