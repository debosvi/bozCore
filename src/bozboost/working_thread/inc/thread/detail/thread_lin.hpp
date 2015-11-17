/*!
 * \file thread_lin.hpp
 * \brief Working Thread linux OS interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#ifndef _BOZ_THREAD_DETAIL_LIN_HPP_
#define _BOZ_THREAD_DETAIL_LIN_HPP_

unsigned int os_get_current_thread_id();
int os_get_priority_value(priority_data::epriority ep);
int os_get_priority_class(priority_data::epriority ep);
unsigned os_adjust_priority(int priority_value, int priority_class, handle_t h);
unsigned os_set_attribute_priority(int priority_value, int priority_class, handle_t h);

#endif // _BOZ_THREAD_DETAIL_LIN_HPP_
