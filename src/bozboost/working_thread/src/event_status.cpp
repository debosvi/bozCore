/*!
 * \file working_thread.hpp
 * \brief Working Thread interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#include "priv/event_status.hpp"

namespace boz {
namespace thread {

event_status::event_status() : 
    success(false), wait(false)
{
    
}

event_status::event_status(bool success_, bool wait_) : 
    success(success_), wait(wait_)
{
    
}

event_status::event_status(bool rc) : 
    success(rc), wait(rc)
{
    
}
  
} // namespace thread
} // namespace boz


