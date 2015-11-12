/*
 * Constantin Fishkin
 * 
 * constantin.fishkin@gmail.com
 *
 */
DWORD os_get_current_thread_id()
{
    return GetCurrentThreadId();
}

int os_get_priority_value(priority_data::epriority ep)
{
    switch(ep)
    {
    case priority_data::NORMAL:
        return THREAD_PRIORITY_NORMAL;
    case priority_data::ABOVE:
        return THREAD_PRIORITY_ABOVE_NORMAL;
    case priority_data::BELOW:
        return THREAD_PRIORITY_BELOW_NORMAL;
    case priority_data::HIGH:
        return THREAD_PRIORITY_HIGHEST;
    case priority_data::LOW:
        return THREAD_PRIORITY_LOWEST;
    // for RT and IDLE the priority_class should be
    // used instead
    case priority_data::RT:
    case priority_data::IDLE:
    default:
        return priority_data::PRIORITY_DEFAULT_VALUE;
    }
}

int os_get_priority_class(priority_data::epriority ep)
{
    switch(ep)
    {
    // for these range
    // the the priority value should be used
    // instead of class
    case priority_data::NORMAL:
    case priority_data::ABOVE:
    case priority_data::BELOW:
    case priority_data::HIGH:
    case priority_data::LOW:
        return priority_data::PRIORITY_DEFAULT_VALUE;
    // for these range - the priority class
    // should be used
    case priority_data::RT:
        return HIGH_PRIORITY_CLASS;
    case priority_data::IDLE:
        return IDLE_PRIORITY_CLASS;
    default:
        return priority_data::PRIORITY_DEFAULT_VALUE;
    }
}

unsigned os_adjust_priority(int priority_value, int priority_class, handle_t h)
{
    if(priority_data::PRIORITY_DEFAULT_VALUE!=priority_value)
        if(!SetThreadPriority(h, priority_value)) return GetLastError();

    if(priority_data::PRIORITY_DEFAULT_VALUE!=priority_class)
        if(!SetPriorityClass(h, priority_class)) return GetLastError();

    return 0;
}

unsigned os_set_attrubute_priority(int priority_value, int priority_class, handle_t h)
{
    // does not work with attrubutes
    return 0;
}
