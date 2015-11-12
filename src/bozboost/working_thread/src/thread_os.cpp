/*
 * Constantin Fishkin
 * 
 * constantin.fishkin@gmail.com
 *
 */
#include "thread/detail/thread_os.h"

namespace qm {
namespace utils {

#if defined(BOOST_THREAD_PLATFORM_WIN32)
    // windows version
    #include "thread/detail/thread_win.hpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
    // pthread version
    #include "thread/detail/thread_lin.hpp"
#else
    #error "Threads are unavailable on this platform"
#endif

}
}

