/**
* \section LICENSE
* Copyright &copy; 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
*
* \section DESCRIPTION
* Header file of class handling the application level functionality of GPIOs
*
* \file database_helper.h
*/

#ifndef _APP_GPIO_H_
#define _APP_GPIO_H_

#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <Lib_GPIO.h>
#include "Lib_tlog.h"
#include <Generic_types_types.h>
#include <schedules.h>


/**
 * \brief GPI table action
 *
 */
typedef enum
{
    gpi_action_noaction = 0,
    gpi_action_delete = 4,
    gpi_action_reboot_processor = 5,
    gpi_action_generate_alarm = 6
}GPI_table_action;

#define GP0_ACTION_NOACTION                0
#define GP0_ACTION_ENROLL_COMPLETED        1
#define GP0_ACTION_ENROLL_INITIATED        2
#define GP0_ACTION_VERIFY_PASS             3
#define GP0_ACTION_VERIFY_FAIL             4
#define GP0_ACTION_FINGER_NOT_DETECTED     5
#define GP0_ACTION_ADMIN_MODE              6
#define GP0_ACTION_ENROLL_MODE             7
#define GP0_ACTION_DELETE_ATTEMPTED        8
#define GP0_ACTION_PROCESSOR_BOOTUP        9
#define GP0_ACTION_FILE_TRANSFER_COMPLETE  10
#define GP0_ACTION_TAMPER_DETECTED         11
#define GP0_ACTION_DURESS_FINGER_DETECTED  12

/**
 * \brief GPO table configuration for which event(s) should toggle which GPO
 */
typedef struct GPO_TABLE
{
    int32_t gpo_0_state; ///< GPO 0 state
    int32_t gpo_1_state; ///< GPO 1 state
    int32_t gpo_0_1_state; ///< GPO 0 and 1 state
    int32_t gpo_2_state; ///< GPO 2 state
} GPO_table;

/**
 * \brief GPI table for configuring an action to be taken by terminal when particular GPI is triggered
 */
typedef struct GPI_TABLE
{
    int32_t gpi_0_state; ///< GPI 0 state
    int32_t gpi_1_state; ///< GPI 1 state
    int32_t gpi_2_state; ///< GPI 2 state
} GPI_table;


/**
 * \brief SDAC alarm type
 *
 */
typedef enum
{
    sdac_alarm_condition_audio_feedback = 0,
    sdac_alarm_condition_led_blinking,
    sdac_alarm_condition_both
} Config_SDAC_alarm_condition;


/**
 * \brief This class handles the application level functionality of GPIOs
 *
 */
class GPIO
{

public:
    /**
     * \brief Threat level setting change callback function pointer signature
     */
    typedef boost::function<void(std::string)> App_threat_level_setting_change_callback_function;

    //! \brief Action to be taken when door help open event is detected
    void handle_door_held_open();

    //! \brief Action to be taken when door forced open event is detected
    void handle_door_forced_open();


private:

    //! shared pointer which contains the instance of class itself
    static boost::shared_ptr<GPIO> instance;

    GPIO_library::Lib_GPIO GPIO1; /**< TODO */

    boost::mutex the_mutex;
    boost::mutex gpio_mutex;
    boost::mutex door_open_schedule_mutex;
    boost::mutex SDAC_GPIO_event_mtx;

    boost::thread thread_door_open_schedule; /**< Thread to monitor door open schedule */
    boost::thread thread_thread_level; /**< Thread to read threat level changes */

    GPO_table m_gpo_table;
    GPI_table m_gpi_table;

    bool m_door_held_open_status;
    bool m_door_forced_open_status;
    std::list<uint32_t> l_sdac_gpio_event_queue;

    int32_t m_door_open_schedule_mode;

    int32_t m_door_open_schedule_door_status;

    int32_t m_schedule_data[NUM_BIOMETRIC_SCHEDULE_ENTRIES][NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY];


    App_threat_level_setting_change_callback_function tl_setting_changed_callback;

    //! \brief Starts the TOM mode by calling the device driver function
    void start_tom();

    void start_tom_signal();

    //! \brief Stops the TOM mode by calling the device driver function
    void stop_tom(bool bManuallyStopped = true);

    void stop_tom_signal(bool bManuallyStopped);

    //! \brief Gets the values of default GPO states from database
    void get_default_gpo_state(int32_t& gpo_0_state, int32_t& gpo_1_state, int32_t& gpo_2_state);

    //! \brief Callback function which calls the appropriate function when an event occurs
    void callback(int32_t status);

    //! \brief Action to be taken when GPO0 finished event is detected
    void handle_gpo0_finished();

    //! \brief Action to be taken when door closed after alarm event is detected
    void handle_door_closed_after_alarm();

    //! \brief Action to be taken GPI 0 is triggered
    void handle_gpi_0_triggered();

    //! \brief Action to be taken GPI 1 is triggered
    void handle_gpi_1_triggered();

    //! \brief Action to be taken GPI 2 is triggered
    void handle_gpi_2_triggered();

    //! \brief Thread function to monitor the door_open_schedule in loop
    void thread_monitor_door_open_schedule();

    //! \brief Thread function to process the door_open_schedule and open SDAC door if required
	void process_door_open_schedule();

	void door_open_schedule_start_signal();

	void door_open_schedule_end_signal();

    //! \brief Thread function to monitor the GPI lines for threat level changes
    void thread_read_thread_level_changes();

    //! \brief Send hide fw message signal in thread
    void hide_fw_message_signal();

    //! \brief Send stop audio signal in thread
    void stop_audio_signal();

    //! \brief Call tl setting change callback
    void call_tl_setting_changed_callback(const std::string &debug);

    //! processes all SDAC or GPIO events
    void SDAC_GPIO_event_processor();

public:
    GPIO();
    ~GPIO();

    //! \brief Method to start thread execution
    void start_thread();

    /**
     * creates instance of the class itself if not created otherwise simply returns the instance
     * \return instance of class
     */
    static boost::shared_ptr<GPIO> & get_instance();

    //! \brief Configures default GPO state by reading the parameters from database and passing them to device driver
    void configure_default_gpo_state(std::string &key_str);

    //! \brief Set the state of a particular GPO line for specific duration
    void set_gpo_state(int32_t line, int32_t state, int32_t duration);

    //! \brief Get the state of all GPO lines
    void get_gpo_state(int32_t& gpo_0_state, int32_t& gpo_1_state, int32_t& gpo_2_state);

    //! \brief Reset the state of a particular GPO line i.e. set them to default state
    void reset_gpo_state(int32_t line);

    //! \brief Get the state of all GPI lines
    void get_gpi_state(int32_t& gpo_0_state, int32_t& gpo_1_state, int32_t& gpo_2_state);

    //! \brief Configures GPO table by reading the parameters from database and passing them to device driver
    void configure_gpo_table(std::string &key_str);

    //! \brief Configures GPI table by reading the parameters from database and passing them to device driver
    void configure_gpi_table(std::string &key_str);

    //! \brief Configures GPI duration by reading the parameters from database and passing them to device driver
    void configure_gpi_duration(std::string &key_str);

    //! \brief Configures GPI active level by reading the parameters from database and passing them to device driver
    void configure_gpi_active_level(std::string &key_str);

    int32_t validate_gpio_mode(const std::string& key,const Distant_cmd::Variant& var);

    int32_t validate_door_open_schedule_mode(const std::string& key,const Distant_cmd::Variant& var);

	//! \brief Configures GPIO mode by reading the parameters from database and passing them to device driver
    void configure_gpio_mode(std::string &key_str);

    //! \brief Unlock door
    int32_t sdac_unlock_door();

    //! \brief Unlock door with user unlock duration
    int32_t sdac_unlock_door(int32_t user_unlock_dur);

    //! \brief Enabled/Disable door open schedule
	int32_t sdac_door_open_schedule_set(bool enable);

    //! \brief Configures TOM mode (Enable/Disable) for SDAC by reading the parameters from database and passing them to device driver
    void configure_tom_mode(std::string &key_str);

    //! \brief Configures the duration value for TOM (time during which door can be kept open) by reading the parameters from database and passing them to device driver
    void configure_tom_dur(std::string &key_str);

    //! \brief Return status of whether TOM is running
    int32_t is_tom_running();

    //! \brief Configures the parameters for SDAC
    void configure_door_controller_params(std::string &key_str);

    //! \brief Configures biometric schedule
    void configure_biometric_schedule(std::string &key_str);

    //! \brief Configures the relay default state
    void configure_relay_default_state(std::string &key_str);

    //! \brief Get the door open status
    void get_door_open_status(int32_t& status);

    //! \brief Get SDAC status
    void get_sdac_status(int32_t& relay_state, int32_t& door_status, int32_t& req_to_exit_state);

    //! \brief Configures device threat level mode by reading the parameters from database and passing them to device driver
    void configure_device_threat_level_mode(std::string &key_str);

    //! \brief Configures command based active threat level by reading the parameters from database and passing them to device driver
    void configure_command_based_active_threat_level(std::string &key_str);

    //! \brief Get the current threat level
    void get_current_threat_level(int32_t& threat_level_mode, int32_t& active_threat_level);

    //! \brief Configures access rule priority by reading the parameters from database and passing them to device driver
    void configure_access_rule_priority(std::string &key_str);

    //! \brief Configures door open schedule by reading the parameters from database and passing them to device driver
    void configure_door_open_schedule(std::string &key_str);

    //! \brief Configures GPI to threat level mapping by reading the parameters from database and passing them to device driver
    void configure_gpi_to_threat_level_mapping(std::string &key_str);

    //! \brief Process the event and toggles the GPO state as per configuration in database
    static void process_event_gpo(const Tlog_info& event);

    /**
     * This function sets Threat level setting change callback function
     *
     * \param[in] callback
     * - Callback function
     */
    void set_threat_level_change_callback(const App_threat_level_setting_change_callback_function &callback);

};

#endif
