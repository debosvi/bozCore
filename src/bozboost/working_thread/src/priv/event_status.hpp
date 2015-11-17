/*!
 * \file working_thread.hpp
 * \brief Working Thread interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#ifndef _BOZ_THREAD_EVTSTATUS_H_
#define _BOZ_THREAD_EVTSTATUS_H_

namespace boz {
namespace thread {

class event_status {
public:
    explicit event_status();
    event_status(bool success_, bool wait_);
    event_status(bool rc);

    bool success;
    bool wait;
};

} // namespace thread
} // namespace boz

#endif // _BOZ_THREAD_EVTSTATUS_H_
