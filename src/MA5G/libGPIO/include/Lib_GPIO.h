/**
 * \section LICENSE
 * Copyright 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * Header file for libary of GPIO/TTL and SDAC(single door access controller)
 *
 * \file Lib_GPIO.h
 *
 */

/*! \mainpage
 * This help contains the documentation for the 'libGPIO' library.
 * Includes the API for configuring the GPIOs of MA1000 board.
 * This page might not be present in the final documentation of the project.
 */

#ifndef _LIB_GPIO_H
#define _LIB_GPIO_H

/****************************************************************
 *   Type and Constant Definitions
 ***************************************************************/

//! SDAC driver device file
#define DEV_SDAC "/dev/sdac"

//! GPIO driver device file
#define DEV_GPIO "/dev/ttl_gpio"

//! GPIO device read size
#define GPIO_DEV_READ_SIZE 4

/****************************************************************
 *   Include Files
 ***************************************************************/
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>



//GPIO
#define GPIO_IOCTL_BASE 'G'
#define BS_CMD_GPI0_READ                   _IOWR(GPIO_IOCTL_BASE, 1, unsigned long)
#define BS_CMD_GPI2_READ                   _IOWR(GPIO_IOCTL_BASE, 3, unsigned long)
#define BS_CMD_GPI1_READ                   _IOWR(GPIO_IOCTL_BASE, 2, unsigned long)
#define BS_CMD_GPI_ACTIVE_LEVEL            _IOWR(GPIO_IOCTL_BASE, 4, unsigned long)
#define BS_CMD_SET_GPIO_MODE               _IOWR(GPIO_IOCTL_BASE, 5, unsigned long)
#define BS_CMD_GET_GPIO_MODE               _IOWR(GPIO_IOCTL_BASE, 6, unsigned long)
#define BS_CMD_SET_GPO_TIMEOUT             _IOWR(GPIO_IOCTL_BASE, 7, unsigned long)
#define BS_CMD_GET_GPIO_SIGNAL_STAT        _IOWR(GPIO_IOCTL_BASE, 8, unsigned long)
#define BS_CMD_GET_GPO_STATUS              _IOWR(GPIO_IOCTL_BASE, 9, unsigned long)
#define BS_CMD_DISABLE_GPI_INTERRUPT       _IOWR(GPIO_IOCTL_BASE,10, unsigned long)
#define BS_CMD_GET_GPI_STATUS              _IOWR(GPIO_IOCTL_BASE,11, unsigned long)
#define BS_CMD_SET_GPO_DEFAULT_STATE       _IOWR(GPIO_IOCTL_BASE,12, unsigned long)
#define BS_CMD_GPO0_READ                   _IOWR(GPIO_IOCTL_BASE,13, unsigned long)
#define BS_CMD_GPO1_READ                   _IOWR(GPIO_IOCTL_BASE,14, unsigned long)
#define BS_CMD_GPO2_READ                   _IOWR(GPIO_IOCTL_BASE,15, unsigned long)
#define BS_CMD_GPI_TRIG_DURATION           _IOWR(GPIO_IOCTL_BASE,16, unsigned long)

//SDAC
#define SDC_IOCTL_BASE 'S'
#define SDC_SET_DOOR_CONTROLLER_MODE       _IOWR(SDC_IOCTL_BASE, 0, unsigned long)
#define SDC_SET_CONFIG_PARAMS              _IOWR(SDC_IOCTL_BASE, 1, unsigned long)
#define SDC_SET_TIMED_OVERRIDE_MODE        _IOWR(SDC_IOCTL_BASE, 2, unsigned long) 
#define SDC_DOOR_UNLOCK                    _IOWR(SDC_IOCTL_BASE, 3, unsigned long)
#define SDC_SET_SCHEDULE_DOOR_CONTROL      _IOWR(SDC_IOCTL_BASE, 4, unsigned long)
#define SDC_SET_TIMED_OVERRIDE_TIMEOUT     _IOWR(SDC_IOCTL_BASE, 5, unsigned long)
#define SDC_SET_RELAY_DEFAULT_STATE        _IOWR(SDC_IOCTL_BASE, 6, unsigned long)
#define SDC_GET_DOOR_STATUS                _IOWR(SDC_IOCTL_BASE, 7, unsigned long)
#define SDC_GET_REQ_TO_EXIT_STATE          _IOWR(SDC_IOCTL_BASE, 8, unsigned long)
#define SDC_GET_RELAY_STATE                _IOWR(SDC_IOCTL_BASE, 9, unsigned long)
// ----------------------------------------------------------

#define DEFAULT_MOTION_EGRESS_TIMEOUT    25
#define DEFAULT_DOOR_UNLOCK_DURATION     25
#define DEFAULT_DOOR_OPEN_HELD_DURATION  25
#define DEFAULT_TOM_DURATION             1

#define MIN_SDC_DOOR_OPEN_DURATION       0
#define MAX_SDC_DOOR_OPEN_DURATION       3600


typedef boost::function<void (int32_t)> gpio_callback_type;

/**
 * \brief namespace GPIO
 *
 */
namespace GPIO_library
{
/**
 * \brief Functional modes of GPIO library
 *
 */
typedef enum
{
    mode_none = -1,
    mode_gpio = 0,
    mode_ttl = 1,
    mode_sdac = 2
} Functional_mode;

/**
 * \brief SDC Signal Reason
 *
 */
typedef enum
{
    sdac_alarm_none = 0,
    sdac_alarm_door_held_open,
    sdac_alarm_gpo0_finished,
    sdac_alarm_forced_open,
    sdac_alarm_door_closed_after_alarm,
    sdac_alarm_door_unlock_after_held_open,
    gpi0_trigger,
    gpi1_trigger,
    gpi2_trigger,
} Action_type;

/**
 * \brief SDAC enabled or disabled
 *
 */
typedef enum
{
    sdac_mode_disable = 0,
    sdac_mode_enable
} Config_SDAC_mode;

/**
 * \brief SDAC request to exit mode
 *
 */
typedef enum
{
    sdac_req_to_exit_none = 0,
    sdac_req_to_exit_push_button,
    sdac_req_to_exit_motion_detector
} Config_SDAC_req_to_exit;


/**
 * \brief SDAC tom enabled or disabled
 *
 */
typedef enum
{
    sdac_tom_disable = 0,
    sdac_tom_enable
} Config_SDAC_tom;

/**
 * \brief
 *
 */
/*typedef enum
 {
 config_sdac_fire_alarm_unlock_disable = 0,
 config_sdac_fire_alarm_unlock_enable
 }Config_SDAC_fire_alarm_unlock;*/

/**
 * \brief Push button door control
 *
 */
typedef enum
{
    pushbutton_manual = 0,
    pushbutton_electric
} Pushbutton_door_control;

/**
 * \brief SDAC internal relay state
 *
 */
typedef enum
 {
    sdac_relay_state_low = 0,
    sdac_relay_state_high
 }Config_SDAC_relay_state;

/**
 * \brief GPI trigger edge
 *
 */
typedef enum
{
    config_gpi_trig_edge_falling = 0,
    config_gpi_trig_edge_rising
} Config_GPI_trig_edge;

/**
 * \brief GPIO state
 *
 */
typedef enum
{
    gpio_state_low = 0,
    gpio_state_high
} GPIO_state;

/**
 * \brief GPI index
 *
 */
typedef enum
{
    gpi0_index = 0,
    gpi1_index = 1,
    gpi2_index = 2,
    gpi_number = 3
} GPI_pin_index;

/**
 * \brief GPO index
 *
 */
typedef enum
{
    gpo0_index = 0,
    gpo1_index = 1,
    gpo2_index = 2,
    gpo_number = 3
} GPO_pin_index;

/**
 * \brief Collection of SDAC configuration parameters
 *
 */
typedef struct SDAC_config_param
{
    uint32_t sdc_enable; /**< For module enable / disable mode */
    uint32_t sdc_door_unlock_dur; /**< Door unlock duration (seconds) */
    uint32_t sdc_max_hldopen_dur; /**< Door held max duration (seconds) */
    uint32_t sdc_req_exit_mode; /**< Request to exit mode */
    uint32_t sdc_motion_egress_tmout; /**< Motion detection timeout (seconds) */
    uint32_t sdc_push_button_mode; /**< Pushbutton mode */
    uint32_t sdc_tom_mode; /**< enable / disable TOM mode */
    uint32_t sdc_tom_dur; /**< Tom duration in mins (minutes) */

    /**
     * \brief
     *
     */
    SDAC_config_param()
    {

        sdc_enable = sdac_mode_disable;
        sdc_door_unlock_dur = 25;
        sdc_max_hldopen_dur = 25;
        sdc_req_exit_mode = sdac_req_to_exit_none;
        sdc_motion_egress_tmout = 25;
        sdc_push_button_mode = pushbutton_manual;
        sdc_tom_mode = sdac_tom_disable;
        sdc_tom_dur = 0;
    }
} SDAC_config_param_t;

/****************************************************************
 *   class Declaration
 ***************************************************************/

/**
 * \brief Library of GPIO/TTL and SDAC(single door access controller).
 *        Provides APIs for configuring GPIO & SDAC parameters.
 *
 */
class Lib_GPIO
{

private:

	gpio_callback_type callback; /**< TODO */

    int32_t m_fd_gpio; /**< GPIO Driver device file */
    int32_t m_fd_sdac; /**< SDAC Driver device file */

    Functional_mode func_mode; /**< Current functional mode of library */

    boost::thread thread_read_sdac; /**< Thread to read the SDAC driver device file */
    boost::thread thread_read_gpi; /**< Thread to read the GPIO driver device file */

    volatile int32_t thread_alive; /**< Flag to start/stop the thread */
    volatile int32_t thread_finish_ack; /**< Ack flag to indicate that the thread has stopped successfully */

    SDAC_config_param_t m_sdac_config; /**< SDAC configuration parameters */

    int32_t m_relay_default_state; /**< SDAC internal relay state when door is closed */
    int32_t m_relay_state; /**< Current SDAC internal relay state */

    //! \brief Open SDAC driver device
    int32_t open_sdac_dev();

    //! \brief Open GPIO driver device
    int32_t open_gpio_dev();

    //! \brief Close SDAC driver device
    int32_t close_sdac_dev();

    //! \brief Close GPIO driver device
    int32_t close_gpio_dev();

    //! \brief Thread function to read the sdac driver device file in loop
    void thread_read_sdac_dev();

    //! \brief Thread function to read the gpio driver device file in loop
    void thread_read_gpi_dev();

    //! \brief Calls the callback function of another class if set using the set_callback() function
    void invoke(int32_t status)
    {
        callback(status);
    }

public:

    //! \brief Constructor
    explicit Lib_GPIO(Functional_mode mode = mode_none);

    //! \brief Destructor
    ~Lib_GPIO();

    //! \brief Select the functional mode to GPIO, SDAC, or TTL.
    int32_t set_functional_mode(Functional_mode mode);

    //! \brief Get the current functional mode (GPIO, SDAC, or TTL)
    Functional_mode get_functional_mode();

    //! \brief Set the SDAC configuration parameters like door unlock duration, door held open duration,
    //!        request to exit mode, motion detector agress timeout, push button door control
    int32_t sdac_set_config(SDAC_config_param_t& sdac_config);

    //! \brief Get the SDAC configuration parameters like door unlock duration, door held open duration,
	//!        request to exit mode, motion detector agress timeout, push button door control
	int32_t sdac_get_config(SDAC_config_param_t& sdac_config);

    //! \brief Set the SDAC relay default state configuration
    int32_t sdac_set_relay_default_state(int32_t relay_default_state);

    //! \brief Unlocks the door for 'door unlock duration' value already set
    int32_t sdac_door_unlock();

    //! \brief Enable/Disable door open schedule
    int32_t sdac_door_open_schedule_set(bool bEnable);

    //! \brief Start the TOM(timed override mode). In this mode the door will be kept
    //!        open for the duration passed as parameter.
    int32_t sdac_start_tom(int32_t duration);

    //! \brief Stop the TOM(timed override mode).
    int32_t sdac_stop_tom();

    //! \brief Returns the status of TOM(timed override mode) i.e. whether active or inactive
    int32_t sdac_is_tom_running();

    //! \brief Get request to exit state
    int32_t sdac_get_req_to_exit_state();

    //! \brief Get door status
    int32_t sdac_get_door_status();

    //! \brief Get door status
    int32_t sdac_get_relay_state();

    //! \brief Set the default state of GPO line to high or low state
    int32_t gpio_set_gpo_default_state(GPO_pin_index line, GPIO_state state);

    //! \brief Set the GPO line to high or low state for a specific duration
    int32_t gpio_set_gpo_state(GPO_pin_index line, GPIO_state state, int32_t duration);

    //! \brief Get the state of GPO line (high or low)
    int32_t gpio_get_gpo_state(GPO_pin_index line, GPIO_state* state);

    //! \brief Set the active level of GPI line
    int32_t gpio_set_gpi_active_level(GPI_pin_index line, GPIO_state state);

    //! \brief Set the trigger duration of GPI line
    int32_t gpio_set_gpi_trig_duration(GPI_pin_index line, int32_t duration);

    //! \brief Get the state of GPI line (high or low)
    int32_t gpio_get_gpi_state(GPI_pin_index line, GPIO_state* state);

    //! \brief Set the callback function to be called when an event like SDAC alarm or GPI trigger occurs
    void set_callback(gpio_callback_type& callback_func)
    {
    	callback = callback_func;
    }

};

}
#endif // _LIB_GPIO_H
