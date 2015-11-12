/*!
 * \file thread_lin.cpp
 * \brief Working Thread linux OS implementation.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "thread_lin.hpp"

unsigned int os_get_current_thread_id()
{
    
#if defined(linux) || defined(__linux) || defined(__linux__)
    // is not cross platform 
    // works only for Linux
    return syscall(SYS_gettid);
#else
    // cross platform
    // actually thread handle
    pthread_t tid = pthread_self();
    unsigned int* ptid = (unsigned int*)&tid;
    return *(unsigned int*)ptid;
#endif
    
}


int os_get_priority_value(priority_data::epriority ep)
{
  static int max_priority=sched_get_priority_max(SCHED_RR);
  static int min_priority=sched_get_priority_min(SCHED_RR);
  static int normal_priority=(max_priority+min_priority)/2;
  static int above_priority=(max_priority+normal_priority)/2;
  static int below_priority=(min_priority+normal_priority)/2;
  static int high_priority=(max_priority+above_priority)/2;
  static int low_priority=(min_priority+below_priority)/2;
     
  switch(ep)
    {
    case priority_data::NORMAL:
        return normal_priority;
    case priority_data::ABOVE:
        return above_priority;
    case priority_data::BELOW:
        return below_priority;
    case priority_data::HIGH:
        return high_priority;
    case priority_data::LOW:
        return low_priority;
    case priority_data::RT:
	return max_priority;
    case priority_data::IDLE:
	return min_priority;
    default:
        return priority_data::PRIORITY_DEFAULT_VALUE;
    }
}

int os_get_priority_class(priority_data::epriority ep)
{
  // scheduler types
  return SCHED_RR;
}

unsigned os_adjust_priority(int priority_value, int priority_class, handle_t h)
{
    // no need for ajustement after thread creation
    return 0;
}

unsigned os_set_attrubute_priority(int priority_value, int priority_class, handle_t h)
{
    // for the default priority we should use SCHED_OTHER class
    // for others, if the class is not specified - SCHED_RR class
    // Otherwise use the specified values
    if(priority_data::PRIORITY_DEFAULT_VALUE==priority_class)
    {
        if(priority_data::PRIORITY_DEFAULT_VALUE==priority_value)
        {
            priority_class=SCHED_OTHER;
            priority_value=0;
        }
        else
        {
            priority_class=SCHED_RR;
        }
    }

    unsigned rc=pthread_attr_setschedpolicy(static_cast<pthread_attr_t*>(h), priority_class);

    if(rc) return rc;

    struct sched_param params;
    memset(&params, 0, sizeof(params));
    params.sched_priority=priority_value;

    return pthread_attr_setschedparam(static_cast<pthread_attr_t*>(h), &params);
}
