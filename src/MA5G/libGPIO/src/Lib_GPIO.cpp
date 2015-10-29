/**
 * \section LICENSE
 * Copyright 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * Implementation file for library of GPIO/TTL and SDAC(single door access controller)
 *
 * \file Lib_GPIO.cpp
 *
 */

/****************************************************************
 *   Include Files
 ***************************************************************/
#include <Lib_GPIO.h>
#include <Common_inc.h>
#include <GPIO_exception.h>
#include <Common_exceptions.h>
#include <sys/ioctl.h>
#include <signal.h>


namespace GPIO_library
{
/****************************************************************
 *   Functions Definition
 ***************************************************************/

/**
 * \brief Constructor
 *
 * \param [in] mode Select GPIO or SDAC mode.
 */
Lib_GPIO::Lib_GPIO(Functional_mode mode)
{
    MA5G_logger::log(PRIORITY_DEBUG, "Lib_GPIO constructor called ...");

    m_sdac_config.sdc_enable = sdac_mode_disable;

    m_sdac_config.sdc_tom_mode = sdac_tom_disable;
    m_sdac_config.sdc_tom_dur = 1; //minutes

    m_relay_default_state = sdac_relay_state_low;
    m_relay_state = 0;

    thread_alive = 0;
    thread_finish_ack = 1;

    m_fd_gpio = -1;
    m_fd_sdac = -1;

    func_mode = mode_none; //Not needed but done to solve parasoft error

    open_gpio_dev();

    set_functional_mode(mode);

    // Initializing below members to remove parasoft violation
    callback = NULL;

}

/**
 * \brief Destructor
 *
 */
Lib_GPIO::~Lib_GPIO()
{
    try
    {
        close_gpio_dev();
        close_sdac_dev();
    }
    catch(GPIO_SDAC_dev_close_failed &e)
    {
        MA5G_logger::log(PRIORITY_ERROR, "GPIO/SDAC device close failed");
    }
}

/**
 * \brief Select the functional mode to GPIO or SDAC.
 *
 * \param [in] mode GPIO or SDAC mode
 * \return int32_t Error code
 */
int32_t Lib_GPIO::set_functional_mode(Functional_mode mode)
{

    if (func_mode == mode)
    {
        MA5G_logger::log(PRIORITY_DEBUG, "GPIO func mode already same \n");
        return 0;
    }

    int32_t iret = 0;

    m_sdac_config.sdc_enable = sdac_mode_disable;

    int32_t prev_func_mode = func_mode;

    func_mode = mode_none;

    if (mode == mode_sdac)
    {

        MA5G_logger::log(PRIORITY_DEBUG, "Setting SDAC mode \n");

        if (1 == thread_alive)
        {
            thread_alive = 0;
            //Added to solve MA5G_SW_PUBLIC-1464 :Open/Close relay crash after 70 iteration
            //Remove this loop : while (!thread_finish_ack){usleep(1000);}
            thread_read_gpi.join();
        }

        thread_alive = 1;

#ifdef ARM
	if(mode_none != prev_func_mode)
	{
        	int32_t gpio_mode = 2;

	        if (ioctl(m_fd_gpio, BS_CMD_SET_GPIO_MODE, gpio_mode) < 0)
        	{
	            throw GPIO_set_mode_failed("Failed to set GPIO mode", __PRETTY_FUNCTION__, __LINE__, "", 0);
        	    return -1;
	        }
	}
#endif //#ifdef ARM

        open_sdac_dev();

        m_sdac_config.sdc_enable = sdac_mode_enable;

#ifdef ARM
        if (ioctl(m_fd_sdac, SDC_SET_DOOR_CONTROLLER_MODE, &m_sdac_config.sdc_enable) < 0)
        {
            throw SDAC_set_mode_failed("Failed to set SDAC mode", __PRETTY_FUNCTION__, __LINE__, "", 0);
            return -1;
        }
#endif //#ifdef ARM

        thread_read_sdac = boost::thread(&Lib_GPIO::thread_read_sdac_dev, this);

        thread_finish_ack = 0;
        func_mode = mode;

    }
    else if (mode == mode_gpio || mode == mode_ttl)
    {

        if (1 == thread_alive)
        {
            thread_alive = 0;

            if (prev_func_mode == mode_sdac)
            {
    #ifdef ARM

                if (ioctl(m_fd_sdac, SDC_SET_DOOR_CONTROLLER_MODE, &m_sdac_config.sdc_enable) < 0)
                {
                    throw SDAC_set_mode_failed("Failed to set SDAC mode", __PRETTY_FUNCTION__, __LINE__, "", 0);
                    return -1;
                }
    #endif //#ifdef ARM
            }

            close_sdac_dev();
            //Added to solve MA5G_SW_PUBLIC-1464 :Open/Close relay crash after 70 iteration
            //Remove:while (!thread_finish_ack){usleep(1000);}
            thread_read_sdac.join();
        }
        else
        {
        	close_sdac_dev();
        }

        thread_alive = 1;

#ifdef ARM
        int32_t gpio_mode = 1;

        if (ioctl(m_fd_gpio, BS_CMD_SET_GPIO_MODE, gpio_mode) < 0)
        {
            throw GPIO_set_mode_failed("Failed to set GPIO mode", __PRETTY_FUNCTION__, __LINE__, "", 0);
            return -1;
        }
#endif //#ifdef ARM

        thread_read_gpi = boost::thread(&Lib_GPIO::thread_read_gpi_dev, this);

        thread_finish_ack = 0;
        func_mode = mode;

    }
    else
    {
        func_mode = mode_none;

#ifdef ARM
        int32_t gpio_mode = 2;

        if (ioctl(m_fd_gpio, BS_CMD_SET_GPIO_MODE, gpio_mode) < 0)
        {
            throw GPIO_set_mode_failed("Failed to set GPIO mode", __PRETTY_FUNCTION__, __LINE__, "", 0);
            return -1;
        }
#endif //#ifdef ARM

        if (mode != mode_none)
        {
            throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "LibGPIO mode", 0);
            return -1;
        }
    }

    return 0;
}


//! \brief Get the current functional mode (GPIO, SDAC, or TTL)
Functional_mode Lib_GPIO::get_functional_mode()
{
    return func_mode;
}


/**
 * \brief Set the SDAC configuration parameters like door unlock duration, door held open duration,
 *        request to exit mode, motion detector agress timeout, push button door control
 *
 * \param [in] sdac_config Object of configuration structure for SDAC parameters
 * \return int Error code
 */
int32_t Lib_GPIO::sdac_set_config(SDAC_config_param_t& sdac_config)
{
    if (func_mode != mode_sdac)
    {
        return -1;
    }

    m_sdac_config.sdc_door_unlock_dur = sdac_config.sdc_door_unlock_dur;
    m_sdac_config.sdc_max_hldopen_dur = sdac_config.sdc_max_hldopen_dur;
    m_sdac_config.sdc_req_exit_mode = sdac_config.sdc_req_exit_mode;
    m_sdac_config.sdc_motion_egress_tmout = sdac_config.sdc_motion_egress_tmout;
    m_sdac_config.sdc_push_button_mode = sdac_config.sdc_push_button_mode;

    MA5G_logger::log(PRIORITY_DEBUG, "Setting door unlock duration to : %d \n", m_sdac_config.sdc_door_unlock_dur);
    MA5G_logger::log(PRIORITY_DEBUG, "Setting max held open duration to : %d \n", m_sdac_config.sdc_max_hldopen_dur);
    MA5G_logger::log(PRIORITY_DEBUG, "Setting request to exit mode to : %d \n", m_sdac_config.sdc_req_exit_mode);
    MA5G_logger::log(PRIORITY_DEBUG, "Setting motion egress timeout to : %d \n", m_sdac_config.sdc_motion_egress_tmout);
    MA5G_logger::log(PRIORITY_DEBUG, "Setting push button mode to : %d \n", m_sdac_config.sdc_push_button_mode);

#ifdef ARM
    if (ioctl(m_fd_sdac, SDC_SET_CONFIG_PARAMS, &m_sdac_config.sdc_enable) < 0)
    {
        throw SDAC_set_params_failed("Failed to set SDAC config params", __PRETTY_FUNCTION__, __LINE__, "", 0);
        return -1;
    }
#endif //#ifdef ARM

    return 0;
}

/**
 * \brief Get the SDAC configuration parameters like door unlock duration, door held open duration,
 *        request to exit mode, motion detector agress timeout, push button door control
 *
 * \param [in] sdac_config Object of configuration structure for SDAC parameters
 * \return int Error code
 */
int32_t Lib_GPIO::sdac_get_config(SDAC_config_param_t& sdac_config)
{
    if (func_mode != mode_sdac)
    {
        return -1;
    }

    sdac_config = m_sdac_config;

    return 0;
}

/**
 * \brief Set the SDAC relay default state configuration
 * \param [in] relay_default_state SDAC internal relay state in door closed state
 * \return int32_t Error code
 */
int32_t Lib_GPIO::sdac_set_relay_default_state(int32_t relay_default_state)
{
    if (func_mode != mode_sdac)
    {
        return -1;
    }

    if (relay_default_state < sdac_relay_state_low || relay_default_state > sdac_relay_state_high)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "SDAC relay state", 0);
        return -1;
    }

    m_relay_default_state = relay_default_state;
    int32_t error = 0;

#ifdef ARM
    error = ioctl(m_fd_sdac, SDC_SET_RELAY_DEFAULT_STATE, &m_relay_default_state);
#endif //#ifdef ARM

    if (error < 0)
    {
        throw SDAC_set_relay_state_failed("Failed to set SDAC relay default state", __PRETTY_FUNCTION__, __LINE__, "", 0);
    }

    return error;

}

/**
 * \brief Set the GPO line to high or low state for a specific duration
 *
 * \param [in] line GPO pin number - 0,1 or 2
 * \param [in] state GPO state - 0(Low) or 1(High)
 * \param [in] duration Duration for which the GPO should remain in state specified
 * \return int Error code
 */
int32_t Lib_GPIO::gpio_set_gpo_state(GPO_pin_index line, GPIO_state state, int32_t duration)
{
    //Commented as need to sent events on GPO even if in SDAC mode
    if (func_mode != mode_gpio)
    {
        return 0;
    }

    if (line < gpo0_index || line > gpo2_index)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPO index", 0);
        return -1;
    }

    if (state < gpio_state_low || state > gpio_state_high)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPO state", 0);
        return -1;
    }

    MA5G_logger::log(PRIORITY_DEBUG, "****** In gpio_set_gpo_state line : %d  state : %d  duration : %d  ***", line, state, duration);

    int32_t gpo_data[2];

    gpo_data[0] = line;
    gpo_data[1] = duration;

    int32_t error = 0;

#ifdef ARM
    error = ioctl(m_fd_gpio, BS_CMD_SET_GPO_TIMEOUT, &gpo_data);

    if (error < 0)
    {
        throw GPIO_set_timeout_failed("Failed to set GPIO timeout", __PRETTY_FUNCTION__, __LINE__, "", 0);
    }
    else
    {
        char buf[2];

        buf[0] = (char) line;
        buf[1] = (char) state;

        int32_t bytes_written = 0;
        bytes_written = write(m_fd_gpio, buf, 2);

        if (bytes_written < 0)
        {
            throw GPIO_set_state_failed("Failed to set GPIO state", __PRETTY_FUNCTION__, __LINE__, "", 0);
        }

    }
#endif//#ifdef ARM

    return error;

}

/** \brief Set the default state of GPO line to high or low state
 *
 * \param [in] line GPO pin number - 0,1 or 2
 * \param [in] state GPO state - 0(Low) or 1(High)
 * \return int Error code
 */
int32_t Lib_GPIO::gpio_set_gpo_default_state(GPO_pin_index line, GPIO_state state)
{
    //Commented as need to sent events on GPO even if in SDAC mode
    if (func_mode != mode_gpio)
    {
        return 0;
    }

    if (line < gpo0_index || line > gpo2_index)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPO index", 0);
        return -1;
    }

    if (state < gpio_state_low || state > gpio_state_high)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPO state", 0);
        return -1;
    }

    int32_t gpo_data[2];

    gpo_data[0] = line;
    gpo_data[1] = state;

    int32_t error = 0;

#ifdef ARM
    error = ioctl(m_fd_gpio, BS_CMD_SET_GPO_DEFAULT_STATE, &gpo_data);
#endif//#ifdef ARM

    if (error < 0)
    {
        throw GPIO_set_default_state_failed("Failed to set GPIO default state", __PRETTY_FUNCTION__, __LINE__, "", 0);
    }

    return error;

}

/**
 * \brief Get the state of GPO line (high or low)
 *
 * \param [in] line GPO pin number - 0,1 or 2
 * \param [out] state State of GPO line got from driver
 * \return int Error code
 */
int32_t Lib_GPIO::gpio_get_gpo_state(GPO_pin_index line, GPIO_state* state)
{
    if (func_mode != mode_gpio)
    {
        return -1;
    }

    if (line < gpo0_index || line > gpo2_index)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPO index", 0);
        return -1;
    }

    switch (line)
    {
    case gpo0_index:
#ifdef ARM
        *state = (GPIO_state) (ioctl(m_fd_gpio, BS_CMD_GPO0_READ, 0));
#endif //#ifdef ARM
        break;

    case gpo1_index:
#ifdef ARM
        *state = (GPIO_state) (ioctl(m_fd_gpio, BS_CMD_GPO1_READ, 0));
#endif //#ifdef ARM
        break;

    case gpo2_index:
#ifdef ARM
        *state = (GPIO_state) (ioctl(m_fd_gpio, BS_CMD_GPO2_READ, 0));
#endif //#ifdef ARM
        break;

    }

    return 0;
}

int32_t Lib_GPIO::gpio_set_gpi_active_level(GPI_pin_index line, GPIO_state state)
{

    if (func_mode != mode_gpio)
    {
        return -1;
    }

    int32_t gpi_data[2];

    gpi_data[0] = line;
    gpi_data[1] = state;

    int32_t error = 0;

#ifdef ARM
    error = ioctl(m_fd_gpio, BS_CMD_GPI_ACTIVE_LEVEL, &gpi_data);

    if (error < 0)
    {
        throw GPIO_set_default_state_failed("Failed to set GPI active level", __PRETTY_FUNCTION__, __LINE__, "", 0);
    }
#endif //#ifdef ARM

    return 0;

}

int32_t Lib_GPIO::gpio_set_gpi_trig_duration(GPI_pin_index line, int32_t duration)
{

    int32_t gpi_data[2];

    gpi_data[0] = line;
    gpi_data[1] = duration;

    int32_t error = 0;

#ifdef ARM
    error = ioctl(m_fd_gpio, BS_CMD_GPI_TRIG_DURATION, &gpi_data);

    if (error < 0)
    {
        throw GPIO_set_timeout_failed("Failed to set GPI trigger duration", __PRETTY_FUNCTION__, __LINE__, "", 0);
    }
#endif //#ifdef ARM

    return 0;

}

/**
 * \brief Get the state of GPI line (high or low)
 *
 * \param [in] line GPI pin number - 0,1 or 2
 * \param [out] state State of GPI line got from driver
 * \return int Error code
 */
int32_t Lib_GPIO::gpio_get_gpi_state(GPI_pin_index line, GPIO_state* state)
{
    if (func_mode != mode_gpio && func_mode != mode_ttl)
    {
        return -1;
    }

    if (line < gpi0_index || line > gpi2_index)
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPI index", 0);
        return -1;
    }

    switch (line)
    {
    case gpi0_index:
#ifdef ARM
        *state = (GPIO_state) (ioctl(m_fd_gpio, BS_CMD_GPI0_READ, 0));
#endif //#ifdef ARM
        break;

    case gpi1_index:
#ifdef ARM
        *state = (GPIO_state) (ioctl(m_fd_gpio, BS_CMD_GPI1_READ, 0));
#endif //#ifdef ARM
        break;

    case gpi2_index:
#ifdef ARM
        *state = (GPIO_state) (ioctl(m_fd_gpio, BS_CMD_GPI2_READ, 0));
#endif //#ifdef ARM
        break;

    default:
        return -1;

    }

    return 0;
}

/**
 * \brief Unlocks the door for 'door unlock duration' value already set
 *
 * \return int Error code
 */
int32_t Lib_GPIO::sdac_door_unlock()
{

    if (func_mode != mode_sdac)
    {
        return -1;
    }

#ifdef ARM
    if (ioctl(m_fd_sdac, SDC_DOOR_UNLOCK, NULL) < 0)
    {
        throw SDAC_door_unlock_failed("Failed to unlock door - SDAC", __PRETTY_FUNCTION__, __LINE__, "", 0);

    }

    m_relay_state = 1;

#endif //#ifdef ARM

    return 0;
}

//! \brief Enable/Disable door open schedule
int32_t Lib_GPIO::sdac_door_open_schedule_set(bool bEnable)
{
    if (func_mode != mode_sdac)
    {
        return -1;
    }

#ifdef ARM

    int32_t mode = 0;

    if(bEnable)
    {
        mode = 1;
    }

    if (ioctl(m_fd_sdac, SDC_SET_SCHEDULE_DOOR_CONTROL, &mode) < 0)
    {
        throw SDAC_door_open_schedule_set_failed("Failed to set door open schedule - SDAC", __PRETTY_FUNCTION__, __LINE__, "", 0);

    }

#endif //#ifdef ARM

    return 0;
}

/**
 * \brief Start the TOM(timed override mode). In this mode the door will be kept
 *        open for the duration passed as parameter.
 *
 * \param [in] duration TOM(timed override mode) timeout value
 * \return int Error code
 */
int32_t Lib_GPIO::sdac_start_tom(int32_t duration)
{
    if (func_mode != mode_sdac)
    {
        return -1;
    }

    int32_t mode = sdac_tom_enable;

    if (duration >= 0)
    {
        m_sdac_config.sdc_tom_mode = mode;
        m_sdac_config.sdc_tom_dur = duration;

        const int32_t secs_per_min = 60;

        int32_t value = duration * secs_per_min;

#ifdef ARM
        if (ioctl(m_fd_sdac, SDC_SET_TIMED_OVERRIDE_TIMEOUT, &value) < 0)
        {
            throw SDAC_set_TOM_timeout_failed("Failed to set TOM timeout - SDAC", __PRETTY_FUNCTION__, __LINE__, "", 0);
            return -1;
        }

        if (ioctl(m_fd_sdac, SDC_SET_TIMED_OVERRIDE_MODE, &mode) < 0)
        {
            throw SDAC_set_TOM_mode_failed("Failed to set TOM mode - SDAC", __PRETTY_FUNCTION__, __LINE__, "", 0);
            return -1;
        }

        if (ioctl(m_fd_sdac, SDC_DOOR_UNLOCK, NULL) < 0)
        {
            throw SDAC_door_unlock_failed("Failed to unlock door - SDAC", __PRETTY_FUNCTION__, __LINE__, "", 0);
            return -1;
        }
#endif //#ifdef ARM

        return 0;
    }
    else
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "TOM duration", 0);
        return -1;
    }
}

/**
 * \brief Stop the TOM(timed override mode).
 *
 * \return int Error code
 */
int32_t Lib_GPIO::sdac_stop_tom()
{
    if (func_mode != mode_sdac)
    {
        return -1;
    }

    int32_t mode = sdac_tom_disable;

    m_sdac_config.sdc_tom_mode = mode;

#ifdef ARM
    if (ioctl(m_fd_sdac, SDC_SET_TIMED_OVERRIDE_MODE, &mode) < 0)
    {
        throw SDAC_set_TOM_mode_failed("Failed to set TOM mode - SDAC", __PRETTY_FUNCTION__, __LINE__, "", 0);
        return -1;
    }
#endif //#ifdef ARM

    return 0;
}

/**
 * \brief Returns the status of TOM(timed override mode) i.e. whether active or inactive
 *
 * \return int  1=active, 0=inactive, -1=error
 */
int32_t Lib_GPIO::sdac_is_tom_running()
{
    return m_sdac_config.sdc_tom_mode;
}

/**
 * \brief Get request to exit state
 *
 * \return int request to exit state
 */
int32_t Lib_GPIO::sdac_get_req_to_exit_state()
{
    int32_t state = -1;

#ifdef ARM
    state = ioctl(m_fd_sdac, SDC_GET_REQ_TO_EXIT_STATE, 0);
#endif //#ifdef ARM

    return state;
}

/**
 * \brief Get door status
 *
 * \return int door status
 */
int32_t Lib_GPIO::sdac_get_door_status()
{
    int32_t state = -1;

#ifdef ARM
    state = ioctl(m_fd_sdac, SDC_GET_DOOR_STATUS, 0);
#endif //#ifdef ARM

    return state;
}

/**
 * \brief Get relay state
 *
 * \return int relay state
 */
int32_t Lib_GPIO::sdac_get_relay_state()
{
    //return m_relay_state;
    int32_t state = -1;

#ifdef ARM
    state = ioctl(m_fd_sdac, SDC_GET_RELAY_STATE, 0);
#endif //#ifdef ARM

    return state;
}

/**
 * \brief Open SDAC driver device
 *
 * \return int Error code
 */
int32_t Lib_GPIO::open_sdac_dev()
{
    int32_t error = 0;
#ifdef ARM
    m_fd_sdac = ::open(DEV_SDAC, O_RDWR);

    if (m_fd_sdac < 0)
    {
        error = -1;
    }
#endif //#ifdef ARM

    return error;
}

/**
 * \brief Open GPIO driver device
 *
 * \return int Error code
 */
int32_t Lib_GPIO::open_gpio_dev()
{
    int32_t error = 0;
#ifdef ARM
    m_fd_gpio = ::open(DEV_GPIO, O_RDWR);

    if (m_fd_gpio < 0)
    {
        error = -1;
    }
#endif //#ifdef ARM
    return error;
}

/**
 * \brief Close SDAC driver device
 *
 * \return int Error code
 */
int32_t Lib_GPIO::close_sdac_dev()
{
    int32_t ret = 0;

#ifdef ARM
    if (m_fd_sdac >= 0)
    {
        ret = ::close(m_fd_sdac);

        if (ret < 0)
        {
            throw GPIO_SDAC_dev_close_failed("Failed to close device", __PRETTY_FUNCTION__, __LINE__, "", 0);
        }

        m_fd_sdac = -1;
    }
#endif //#ifdef ARM

    return ret;

}

/**
 * \brief Close GPIO driver device
 *
 * \return int Error code
 */
int32_t Lib_GPIO::close_gpio_dev()
{
    int32_t ret = 0;

#ifdef ARM
    if (m_fd_gpio >= 0)
    {
        ret = ::close(m_fd_gpio);

        if (ret < 0)
        {
            throw GPIO_SDAC_dev_close_failed("Failed to close device", __PRETTY_FUNCTION__, __LINE__, "", 0);
        }

        m_fd_gpio = -1;
    }
#endif //#ifdef ARM

    return ret;

}

/**
 * \brief Thread function to read the sdac driver device file in loop
 *
 * \param [in] obj Pointer to the class object (this)
 */
void Lib_GPIO::thread_read_sdac_dev()
{
    int32_t ret = 0;
    char buf[sizeof(int32_t)];

    int32_t alarm_type;
    char tmp_data[sizeof(alarm_type)];

    MA5G_logger::log(PRIORITY_DEBUG, "Thread started SDAC ... \n");

    while (thread_alive)
    {
        usleep(1000);

#ifdef ARM
        memset(buf, '\0', sizeof(int32_t));

        ret = ::read(m_fd_sdac, buf, sizeof(int32_t));

        memcpy(&alarm_type, buf, sizeof(alarm_type));

        MA5G_logger::log(PRIORITY_DEBUG, "******** Got an event : %d \n", alarm_type);

        if (ret == 0)
        {
            //No new data read. Do nothing. Continue loop
        }
        else if (ret > 0)
        {
            //TODO: Call callback function here

            if (alarm_type == sdac_alarm_gpo0_finished)
            {
                m_relay_state = 0;
                //sdac_stop_tom();
            }

            invoke(alarm_type);
        }
        else if (ret < 0)
        {
            //TODO : Handle error
            break;
        }
#endif //#ifdef ARM

    }

    thread_finish_ack = 1;
}

/**
 * \brief Thread function to read the gpio driver device file in loop
 *
 * \param [in] obj Pointer to the class object (this)
 */
void Lib_GPIO::thread_read_gpi_dev()
{
    int32_t ret = 0;
    char buf[GPIO_DEV_READ_SIZE];

    MA5G_logger::log(PRIORITY_DEBUG, "Thread started GPIO ... \n");

    while (thread_alive)
    {
        usleep(1000);

#ifdef ARM
        memset(buf, 0, GPIO_DEV_READ_SIZE);

        ret = ::read(m_fd_gpio, buf, GPIO_DEV_READ_SIZE);

        if (ret == 0)
        {
            //No new data read. Do nothing. Continue loop
        }
        else if (ret > 0)
        {
            MA5G_logger::log(PRIORITY_DEBUG, "Signal status : %d Index : %d Level : %d Edge : %d", buf[0],
                                                                                    buf[1], buf[2], buf[3]);

            if(0 == buf[0])
            {
                if (gpi0_index == buf[1])
                {
                    invoke(gpi0_trigger);
                }
                else if (gpi1_index == buf[1])
                {
                    invoke(gpi1_trigger);
                }
                else if (gpi2_index == buf[1])
                {
                    invoke(gpi2_trigger);
                }
            }
        }
        else if (ret < 0)
        {
            //TODO : Handle error
            break;
        }
#endif //#ifdef ARM

    }

    thread_finish_ack = 1;
}

}
