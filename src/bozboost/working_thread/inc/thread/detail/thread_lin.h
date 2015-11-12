/*
 * Constantin Fishkin
 * 
 * constantin.fishkin@gmail.com
 *
 */
#ifndef QM_THREAD_DETAIL_LIN_HPP
#define QM_THREAD_DETAIL_LIN_HPP

unsigned int os_get_current_thread_id();
int os_get_priority_value(priority_data::epriority ep);
int os_get_priority_class(priority_data::epriority ep);
unsigned os_adjust_priority(int priority_value, int priority_class, handle_t h);
unsigned os_set_attrubute_priority(int priority_value, int priority_class, handle_t h);

#endif
