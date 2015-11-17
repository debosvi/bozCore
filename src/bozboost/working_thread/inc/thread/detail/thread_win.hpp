/*!
 * \file thread_win.hpp
 * \brief Working Thread windows OS interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */

#ifndef _BOZ_THREAD_DETAIL_WIN_HPP_
#define _BOZ_THREAD_DETAIL_WIN_HPP_

#include <windows.h>

DWORD os_get_current_thread_id();
int os_get_priority_value(priority_data::epriority ep);
int os_get_priority_class(priority_data::epriority ep);
unsigned os_adjust_priority(int priority_value, int priority_class, handle_t h);
unsigned os_set_attribute_priority(int priority_value, int priority_class, handle_t h);

#endif // _BOZ_THREAD_DETAIL_WIN_HPP_
