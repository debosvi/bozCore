/**
 * \section LICENSE
 * Copyright &copy; 2012 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * This class handles the application level functionality of GPIOs
 *
 * \file gpio.cpp
 */

#include <gpio.h>
#include <GPIO_exception.h>
#include <database_helper.h>
#include <iostream>
#include <Validate_Parameter.h>
#include <MA5G_logger.h>
#include <MA5G_param_keys.h>
#include <Config_db.h>
#include <Lib_tlog.h>
#include <Transaction_log.h>
#include <Event_manager.h>
#include <MA5G_firmware.h>
#include <Threat_level.h>
#include <License_tools.h>

using namespace std;
using namespace GPIO_library;

boost::shared_ptr<GPIO> GPIO::instance;

/**
 * \brief Constructor
 *
 */
GPIO::GPIO() // parasoft-suppress OOP-08 "_1, _2, etc. are Boost library's global variables"
{
	m_door_open_schedule_mode = 0;

    m_door_open_schedule_door_status = 0;

    m_door_held_open_status = false;

    m_door_forced_open_status = false;

	gpio_callback_type callback_to_set = boost::bind(&GPIO::callback,this,_1);
    GPIO1.set_callback(callback_to_set);

    callback_fun callback_to_set_1 = boost::bind(&GPIO::configure_default_gpo_state,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_0_DEFAULT_STATE,callback_to_set_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_1_DEFAULT_STATE,callback_to_set_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_2_DEFAULT_STATE,callback_to_set_1);

    callback_fun callback_to_set_2 = boost::bind(&GPIO::configure_gpo_table,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_TABLE_GPO_0,callback_to_set_2);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_TABLE_GPO_1,callback_to_set_2);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_TABLE_GPO_0_AND_1,callback_to_set_2);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPO_TABLE_GPO_2,callback_to_set_2);

    callback_fun callback_to_set_4 = boost::bind(&GPIO::configure_gpi_table,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPI_TABLE_GPI_0,callback_to_set_4);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPI_TABLE_GPI_1,callback_to_set_4);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPI_TABLE_GPI_2,callback_to_set_4);

    callback_fun callback_to_set_5 = boost::bind(&GPIO::configure_gpi_duration,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPI_DURATION,callback_to_set_5);

    callback_fun callback_to_set_6 = boost::bind(&GPIO::configure_gpi_active_level,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_GPI_ACTIVE_LEVEL,callback_to_set_6);

    callback_fun callback_to_set_9 = boost::bind(&GPIO::configure_gpio_mode,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_FUNC_MODE,callback_to_set_9);

    validation_callback_fun callback_to_set_validation;
    callback_to_set_validation = boost::bind(&GPIO::validate_gpio_mode,this,_1,_2);
    MA5G_firmware::install_validation_listener(P_GPIO_FUNC_MODE, callback_to_set_validation);

    callback_fun callback_to_set_10 = boost::bind(&GPIO::configure_tom_mode,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_TOM_MODE,callback_to_set_10);

    callback_fun callback_to_set_11 = boost::bind(&GPIO::configure_tom_dur,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_TOM_DURATION,callback_to_set_11);

    callback_fun callback_to_set_12 = boost::bind(&GPIO::configure_device_threat_level_mode,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_THREAT_LEVEL_MODE,callback_to_set_12);

    callback_fun callback_to_set_13 = boost::bind(&GPIO::configure_command_based_active_threat_level,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_CMD_BASED_ACTIVE_THREAT_LEVEL,callback_to_set_13);

    callback_fun callback_to_set_14 = boost::bind(&GPIO::configure_access_rule_priority,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_ACCESS_RULE_PRIORITY_1,callback_to_set_14);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_ACCESS_RULE_PRIORITY_2,callback_to_set_14);

    callback_fun callback_to_set_15 = boost::bind(&GPIO::configure_door_open_schedule,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_DOOR_OPEN_SCHEDULE,callback_to_set_15);

    validation_callback_fun callback_to_set_validation_1;
    callback_to_set_validation_1 = boost::bind(&GPIO::validate_door_open_schedule_mode,this,_1,_2);
    MA5G_firmware::install_validation_listener(P_GPIO_DOOR_OPEN_SCHEDULE, callback_to_set_validation_1);

    callback_fun callback_to_set_16 = boost::bind(&GPIO::configure_gpi_to_threat_level_mapping,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_THREAT_LEVEL_GPI_0,callback_to_set_16);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_THREAT_LEVEL_GPI_1,callback_to_set_16);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_THREAT_LEVEL_GPI_2,callback_to_set_16);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_THREAT_LEVEL_GPI_3,callback_to_set_16);

    callback_fun callback_to_set_18 = boost::bind(&GPIO::configure_relay_default_state,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_SDAC_RELAY_DEFAULT_STATE,callback_to_set_18);


    callback_fun callback_to_set_19 = boost::bind(&GPIO::configure_door_controller_params,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_SDAC_DOOR_UNLOCK_DUR,callback_to_set_19);

    callback_fun callback_to_set_20 = boost::bind(&GPIO::configure_door_controller_params,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_SDAC_MAX_DOOR_HELD_OPEN_DUR,callback_to_set_20);

    callback_fun callback_to_set_21 = boost::bind(&GPIO::configure_door_controller_params,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_SDAC_RTE_MODE,callback_to_set_21);

    callback_fun callback_to_set_22 = boost::bind(&GPIO::configure_door_controller_params,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_SDAC_RTE_EGRESS_TIMEOUT,callback_to_set_22);

    callback_fun callback_to_set_23 = boost::bind(&GPIO::configure_door_controller_params,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_GPIO_SDAC_PUSH_BTN_MODE,callback_to_set_23);

    callback_fun callback_to_set_24 = boost::bind(&GPIO::configure_biometric_schedule,this,_1);
    MA5G_firmware::p_config_db->install_listener(P_BIOMETRIC_SCHEDULE,callback_to_set_24);


    try
    {

        std::string key_str = std::string(P_GPIO_GPO_0_DEFAULT_STATE);

        configure_default_gpo_state(key_str);


        key_str = std::string(P_GPIO_GPO_1_DEFAULT_STATE);

        configure_default_gpo_state(key_str);


        key_str = std::string(P_GPIO_GPO_2_DEFAULT_STATE);

        configure_default_gpo_state(key_str);

    }
    catch(...)
    {

    }

    try
    {
        std::string str;
        configure_gpio_mode(str);

        //Disabling TOM mode in database on every reset
        int32_t tom_mode = sdac_tom_disable;

        int32_t retval = Database_helper::set_val_in_db(P_GPIO_TOM_MODE, &tom_mode, db_unsigned_int, sizeof(uint32_t));

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to set tom mode in database from constructor, return: %d", retval);
            return;
        }


        configure_door_controller_params(str);

        configure_relay_default_state(str);

    }
    catch (SDAC_dev_open_failed &e)
    {

    }
    catch (SDAC_set_mode_failed &e)
    {

    }
    catch (GPIO_dev_open_failed &e)
    {

    }
    catch (GPIO_set_mode_failed &e)
    {

    }
    catch (Invalid_parameter_value &e)
    {

    }
    catch (GPIO_invalid_mode_for_function &e)
    {

    }

    this->tl_setting_changed_callback = NULL;


	int32_t mode;

	int32_t retval;

	retval = Database_helper::get_val_from_db((char*) P_GPIO_DOOR_OPEN_SCHEDULE, &mode);

	if (SUCCESS != retval)
	{
		MA5G_logger::log(PRIORITY_ERROR, "Failed to get door open schedule, return: %d", retval);
		return;
	}

	m_door_open_schedule_mode = mode;


	retval = Database_helper::get_val_from_db((char *) P_BIOMETRIC_SCHEDULE, (int32_t*)m_schedule_data[0]);

	if (SUCCESS != retval)
	{
		MA5G_logger::log(PRIORITY_ERROR, "Failed to get biometric schedule, return: %d", retval);
		return;
	}

}

/**
 * \brief Destructor
 *
 */
GPIO::~GPIO()
{

}

void GPIO::start_thread()
{
    //Start door open schedule thread
    thread_door_open_schedule = boost::thread(&GPIO::thread_monitor_door_open_schedule, this);
}

// creates instance of the class itself if not created otherwise simply returns the instance
boost::shared_ptr<GPIO> & GPIO::get_instance()
{
    if (!instance.use_count())
    {

        instance = boost::shared_ptr<GPIO>(new GPIO());
    }
    return instance;
}

//! \brief Starts the TOM mode by calling the device driver function
void GPIO::start_tom()
{
    if(!MA5G_firmware::is_MA5xx_protocol())
    {
        int32_t retval;

        if (GPIO1.sdac_is_tom_running() == sdac_tom_enable)
        {
            //TODO : error, TOM already running
            cout << "*** TOM already running ***" << endl;
        }
        else
        {
            int32_t duration;

            retval = Database_helper::get_val_from_db((char*) P_GPIO_TOM_DURATION, &duration);

            if (SUCCESS != retval)
            {
                MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom duration, return: %d", retval);
                return;
            }
            else
            {
            	boost::mutex::scoped_lock lock(the_mutex);

                cout << "*** Starting TOM ***" << endl;

                retval = GPIO1.sdac_start_tom(duration);
                if (SUCCESS != retval)
                {
                    MA5G_logger::log(PRIORITY_ERROR, "Failed to set tom mode, return: %d", retval);
                    return;
                }

            	m_door_held_open_status = false;
        		m_door_forced_open_status = false;

                boost::thread thread_hide_fw_message = boost::thread(&GPIO::hide_fw_message_signal, this);

                boost::thread thread_stop_audio = boost::thread(&GPIO::stop_audio_signal, this);

    //		    Transaction_log::transaction_log_write( Lib_tlog::tlac_door_unlocked_tom,NULL,0,Lib_tlog::tlfp_io_channel,
    //		    		0,NULL,NULL,NULL,NULL,0,0,0,0,NULL);

                boost::thread thread_map = boost::thread(&GPIO::start_tom_signal, this);

                /*Tlog_info tlinfo;
                tlinfo.action = Lib_tlog::tlac_door_unlocked;
                tlinfo.channel = Lib_tlog::tlfp_io_channel;
                Event_manager::send_event(tlinfo);*/

    //		    Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
    //		    dbus_obj->send_gui_signal_tom_started();

                cout << "*** TOM started ***" << endl;
            }
        }
    }

}

void GPIO::start_tom_signal()
{
    boost::mutex::scoped_lock lock(gpio_mutex);
    Tlog_info tlinfo;
    tlinfo.action = Lib_tlog::tlac_door_unlocked;
    tlinfo.channel = Lib_tlog::tlfp_io_channel;
    Event_manager::send_event(tlinfo);
}

//! \brief Stops the TOM mode by calling the device driver function
void GPIO::stop_tom(bool bManuallyStopped)
{
    cout << "*** stop_tom called ***" << endl;

	int32_t retval;

	if (sdac_tom_disable == GPIO1.sdac_is_tom_running())
	{
		//TODO : error, TOM not running
	    cout << "*** TOM not running ***" << endl;
	}
	else if (sdac_tom_enable == GPIO1.sdac_is_tom_running())
	{

		boost::mutex::scoped_lock lock(the_mutex);

	    cout << "*** Stopping TOM ***" << endl;

		retval = GPIO1.sdac_stop_tom();

		if (SUCCESS != retval) // parasoft-suppress BD-PB-CC "Condition is dependent on 'ARM' macro"
		{
			MA5G_logger::log(PRIORITY_ERROR, "Failed to set tom mode, return: %d", retval);
			return;
		}


	    int32_t tom_mode = 0;

	    retval = Database_helper::set_val_in_db(P_GPIO_TOM_MODE, &tom_mode, db_unsigned_int, sizeof(uint32_t));

	    if (SUCCESS != retval)
	    {
	        MA5G_logger::log(PRIORITY_ERROR, "Failed to set tom mode in database, return: %d", retval);
	        return;
	    }


	    boost::thread thread_map = boost::thread(&GPIO::stop_tom_signal, this, bManuallyStopped);

        cout << "*** TOM stopped ***" << endl;
	}
}

void GPIO::stop_tom_signal(bool bManuallyStopped)
{
    Tlog_info tlinfo;
    tlinfo.action = Lib_tlog::tlac_door_locked_back;

    if(bManuallyStopped)
    {
        cout << "*** TOM stopped manually ***" << endl;
        tlinfo.action_data[Lib_tlog::tlog_action_data_0] = Lib_tlog::tlog_data_0_TOM_stopped;
    }
    else
    {
        cout << "*** TOM stopped as expired ***" << endl;
        tlinfo.action_data[Lib_tlog::tlog_action_data_0] = Lib_tlog::tlog_data_0_TOM_expired;
    }

    tlinfo.channel = Lib_tlog::tlfp_io_channel;

    Event_manager::send_event(tlinfo);
}

//! \brief Gets the values of default GPO states from database
void GPIO::get_default_gpo_state(int32_t& gpo_0_state, int32_t& gpo_1_state, int32_t& gpo_2_state)
{
    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_0_DEFAULT_STATE, &gpo_0_state);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo 0 default state, return: %d", retval);
        return;
    }

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_1_DEFAULT_STATE, &gpo_1_state);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo 1 default state, return: %d", retval);
        return;
    }

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_2_DEFAULT_STATE, &gpo_2_state);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo 2 default state, return: %d", retval);
        return;
    }
}

/**
 * \brief
 *
 * \param vself
 * \param status
 */
void GPIO::callback(int32_t status)
{
	MA5G_logger::log(PRIORITY_DEBUG, "%s:%d received event=%d gpio_mode=%d",__func__,__LINE__,status,GPIO1.get_functional_mode());
	
    if (status == sdac_alarm_door_held_open)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(sdac_alarm_door_held_open);
        l_sdac_gpio_event_queue.push_back(sdac_alarm_door_held_open);
    }
    else if (status == sdac_alarm_gpo0_finished)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(sdac_alarm_gpo0_finished);
        l_sdac_gpio_event_queue.push_back(sdac_alarm_gpo0_finished);
    }
    else if (status == sdac_alarm_forced_open)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(sdac_alarm_forced_open);
        l_sdac_gpio_event_queue.push_back(sdac_alarm_forced_open);
    }
    else if (status == sdac_alarm_door_closed_after_alarm)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(sdac_alarm_door_closed_after_alarm);
        l_sdac_gpio_event_queue.push_back(sdac_alarm_door_closed_after_alarm);
    }
    else if (status == sdac_alarm_door_unlock_after_held_open)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(sdac_alarm_door_unlock_after_held_open);
        l_sdac_gpio_event_queue.push_back(sdac_alarm_door_unlock_after_held_open);
    }
    else if (status == sdac_alarm_none)
    {
        cout << "No SDAC Alarm\n";
    }
    else if (status == gpi0_trigger)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(gpi0_trigger);
        l_sdac_gpio_event_queue.push_back(gpi0_trigger);
    }
    else if (status == gpi1_trigger)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(gpi1_trigger);
        l_sdac_gpio_event_queue.push_back(gpi1_trigger);
    }
    else if (status == gpi2_trigger)
    {
        boost::mutex::scoped_lock SDAC_GPIO_event_lock(SDAC_GPIO_event_mtx);
        l_sdac_gpio_event_queue.remove(gpi2_trigger);
        l_sdac_gpio_event_queue.push_back(gpi2_trigger);
    }
    else
    {
        cout << "Invalid status number !!\n";
    }
}

void GPIO::SDAC_GPIO_event_processor()
{
    const uint32_t idle_sleep_ms = 25;
    const uint32_t gui_sleep_ms = 500;
    uint32_t i_event_to_process = 0;

    while(true)
    {
        // wait for any event to occure
        if(l_sdac_gpio_event_queue.empty())
        {
	        boost::this_thread::sleep(boost::posix_time::milliseconds(idle_sleep_ms));
            continue;
        }
        else
        {
            boost::mutex::scoped_lock door_open_schedule_lock(SDAC_GPIO_event_mtx);
            i_event_to_process = l_sdac_gpio_event_queue.front();
            l_sdac_gpio_event_queue.pop_front();
        }

        switch (i_event_to_process)
        {
        case sdac_alarm_door_held_open:
        {
            m_door_held_open_status = true;
            try
            {
                //boost::thread thread_map = boost::thread(&GPIO::handle_door_held_open, this);
                handle_door_held_open();
                //thread_map.detach();
            } catch (std::exception& e)
            {
                // ignore
                MA5G_logger::log(PRIORITY_ERROR, "%s:%d  ERROR:%s", __PRETTY_FUNCTION__, __LINE__, e.what());
            }
        }
            break;
        case sdac_alarm_gpo0_finished:
        {
            handle_gpo0_finished();
        }
            break;
        case sdac_alarm_forced_open:
        {
            m_door_forced_open_status = true;
            try
            {
                //boost::thread thread_map = boost::thread(&GPIO::handle_door_forced_open, this);
                handle_door_forced_open();
                //thread_map.detach();
            } catch (std::exception& e)
            {
                // ignore
                MA5G_logger::log(PRIORITY_ERROR, "%s:%d  ERROR:%s", __PRETTY_FUNCTION__, __LINE__, e.what());
            }
        }
            break;
        case sdac_alarm_door_closed_after_alarm:
        {
            if (m_door_held_open_status == true || m_door_forced_open_status == true)
            {
                try
                {
                    //boost::thread thread_map = boost::thread(&GPIO::handle_door_closed_after_alarm, this);
                    handle_door_closed_after_alarm();
                    //thread_map.detach();
                } catch (std::exception& e)
                {
                    // ignore
                    MA5G_logger::log(PRIORITY_ERROR, "%s:%d  ERROR:%s", __PRETTY_FUNCTION__, __LINE__, e.what());
                }
            }

            m_door_held_open_status = false;
            m_door_forced_open_status = false;
        }
            break;
        case sdac_alarm_door_unlock_after_held_open:
        {
            m_door_held_open_status = false;
            m_door_forced_open_status = false;

            uint32_t tamper_status;
            MA5G_firmware::app_lib_object->p_tamper->get_tamper_status(&tamper_status);
            if (tamper_status != Tamper::tamper_occurred)
            {
                //boost::thread thread_hide_fw_message = boost::thread(&GPIO::hide_fw_message_signal, this);
                //boost::thread thread_stop_audio = boost::thread(&GPIO::stop_audio_signal, this);
                hide_fw_message_signal();
                stop_audio_signal();
            }
        }
            break;
        case gpi0_trigger:
        {
            handle_gpi_0_triggered();
        }
            break;
        case gpi1_trigger:
        {
            handle_gpi_1_triggered();
        }
            break;
        case gpi2_trigger:
        {
            handle_gpi_2_triggered();
        }
        break;
        default:
            break;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(gui_sleep_ms));
    }
}

/**
 * \brief
 *
 */
void GPIO::handle_door_held_open()
{
    if(!MA5G_firmware::is_MA5xx_protocol())
    {

    	if(m_door_held_open_status == true)
    	{
			cout << "In handle door held open \n";

		//    Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
		//    dbus_obj->gui_send_event_door_held_open();
		//
		//    dbus_obj->send_gui_signal_door_held_open();

		//    Transaction_log::transaction_log_write(Lib_tlog::tlac_door_opened_for_too_long, NULL, 0, Lib_tlog::tlfp_io_channel,
		//            0, NULL, NULL, NULL, NULL, 0, 0, 0, 0, NULL);
			Tlog_info tlinfo;
			tlinfo.action = Lib_tlog::tlac_door_opened_for_too_long;
			tlinfo.channel = Lib_tlog::tlfp_io_channel;
			Event_manager::send_event(tlinfo);

    	}

    }

}

/**
 * \brief
 *
 */
void GPIO::handle_gpo0_finished()
{
	cout << "In handle gpo0 finished \n";
	stop_tom(false);

    std:string key_str;
    configure_door_controller_params(key_str);
}

/**
 * \brief
 *
 */
void GPIO::handle_door_forced_open()
{
    if(!MA5G_firmware::is_MA5xx_protocol())
    {

    	if(m_door_forced_open_status == true)
    	{
			cout << "In handle door forced open \n";

		//    Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
		//    dbus_obj->gui_send_event_door_forced_open();
		//
		//    dbus_obj->send_gui_signal_door_forced_open();

		//    Transaction_log::transaction_log_write(Lib_tlog::tlac_door_forced_open, NULL, 0, Lib_tlog::tlfp_io_channel, 0, NULL,
		//            NULL, NULL, NULL, 0, 0, 0, 0, NULL);
			Tlog_info tlinfo;
			tlinfo.action = Lib_tlog::tlac_door_forced_open;
			tlinfo.channel = Lib_tlog::tlfp_io_channel;
			Event_manager::send_event(tlinfo);

    	}

    }
}

/**
 * \brief
 *
 */
void GPIO::handle_door_closed_after_alarm()
{
    if(!MA5G_firmware::is_MA5xx_protocol())
    {
        cout << "In handle door closed after alarm \n";

        if (0 == m_door_open_schedule_door_status)
        {
        //    Transaction_log::transaction_log_write(Lib_tlog::tlac_door_closed_after_alarm, NULL, 0, Lib_tlog::tlfp_io_channel,
        //            0, NULL, NULL, NULL, NULL, 0, 0, 0, 0, NULL);
            Tlog_info tlinfo;
            tlinfo.action = Lib_tlog::tlac_door_closed_after_alarm;
            tlinfo.channel = Lib_tlog::tlfp_io_channel;
            Event_manager::send_event(tlinfo);
        }

    //    Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
    //    dbus_obj->send_gui_signal_door_closed_after_alarm();
    //    dbus_obj->hide_fw_message();
    }
}


/**
 * \brief Thread function to monitor the door_open_schedule in loop
 *
 */
void GPIO::thread_monitor_door_open_schedule()
{
    while(1)
    {
        sleep(5);

        process_door_open_schedule();

    }
}

/**
 * \brief Thread function to process the door_open_schedule and open SDAC door if required
 *
 */
void GPIO::process_door_open_schedule()
{

	boost::mutex::scoped_lock door_open_schedule_lock(door_open_schedule_mutex);

    int32_t open_door = 0;


    if (1 == m_door_open_schedule_mode)
    {

        //cout << "Processing door open schedule ............ " << endl;


        int32_t schedule_byte[NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY];

        boost::posix_time::ptime curr_time(boost::posix_time::second_clock::local_time());

        //cout << "curr_time : " << curr_time << endl;

        memcpy(schedule_byte, m_schedule_data[curr_time.date().day_of_week()], sizeof(schedule_byte));

        int32_t devider[NUM_OF_DEVIDER*NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY];
        int32_t midnight_bit_flag = 0;

        int32_t time_slot_hour[NUM_OF_DEVIDER*NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY] = {0};
        memset(time_slot_hour, 0, sizeof(time_slot_hour));

        int32_t time_slot_min[NUM_OF_DEVIDER*NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY]  = {0};
        memset(time_slot_min, 0, sizeof(time_slot_min));

        midnight_bit_flag = schedule_byte[0] & MIDNIGHT_BIT_MASK;

        int32_t valid_dividers = NUM_OF_DEVIDER*NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY;

        if (schedule_byte[0] != 0xFFFFFFFF)
        {
            for(int32_t i=0;i<NUM_OF_DEVIDER*NUM_WORD_IN_BIOMETRIC_SCHEDULE_ENTRY;++i)
            {

                devider[i] = (schedule_byte[i/4] & (DEVIDER0_MASK >> (i%4)*EIGHT_BITS)) >> ((3-(i%4))*EIGHT_BITS);

                if (devider[i] > 96)
                {
                    valid_dividers = i;
                    break;
                }

                time_slot_hour[i] = devider[i] / DEVIDER_TO_GET_HOUR ;
                time_slot_min[i] = (devider[i]%4) * MULTIPLIER_TO_GET_MIN ;

            }

            for(int32_t i=0;i<(valid_dividers-1);i=i+2)
            {
                boost::posix_time::ptime schedule_time1(curr_time.date(),boost::posix_time::time_duration(time_slot_hour[i],time_slot_min[i],0));
                //cout << "-- schedule_time : " << schedule_time1 << endl;

                boost::posix_time::ptime schedule_time2(curr_time.date(),boost::posix_time::time_duration(time_slot_hour[i+1],time_slot_min[i+1],0));
                //cout << "-- schedule_time : " << schedule_time2 << endl;

                if (schedule_time1 <= curr_time && schedule_time2 >= curr_time)
                {
                	open_door = 1;
                }
            }

        }

        if (1 == open_door)
        {
            if (0 == m_door_open_schedule_door_status)
            {
                cout << "-- Opening door from door open schedule " << endl;
                m_door_open_schedule_door_status = 1;
                sdac_door_open_schedule_set(true);

            	m_door_held_open_status = false;
        		m_door_forced_open_status = false;

                boost::thread thread_hide_fw_message = boost::thread(&GPIO::hide_fw_message_signal, this);
                boost::thread thread_stop_audio = boost::thread(&GPIO::stop_audio_signal, this);
                boost::thread thread_door_open_schedule_start = boost::thread(&GPIO::door_open_schedule_start_signal, this);

            }
        }
        else
        {
            if (1 == m_door_open_schedule_door_status)
            {
                cout << "-- Closing door from door open schedule " << endl;
                m_door_open_schedule_door_status = 0;
                sdac_door_open_schedule_set(false);

                boost::thread thread_map = boost::thread(&GPIO::door_open_schedule_end_signal, this);
            }
        }

    }
    else
    {
        if (1 == m_door_open_schedule_door_status)
        {
            cout << "-- Closing door from door open schedule " << endl;
            m_door_open_schedule_door_status = 0;
            sdac_door_open_schedule_set(false);

            boost::thread thread_map = boost::thread(&GPIO::door_open_schedule_end_signal, this);
        }
    }

}

void GPIO::door_open_schedule_start_signal()
{
    boost::mutex::scoped_lock lock(gpio_mutex);
    Tlog_info tlinfo;
    tlinfo.action = Lib_tlog::tlac_door_unlocked;
    tlinfo.action_data[Lib_tlog::tlog_action_data_0] = Lib_tlog::tlog_data_0_door_open_schedule_start;
    tlinfo.channel = Lib_tlog::tlfp_io_channel;

    Event_manager::send_event(tlinfo);
}
void GPIO::door_open_schedule_end_signal()
{
    boost::mutex::scoped_lock lock(gpio_mutex);
    Tlog_info tlinfo;
    tlinfo.action = Lib_tlog::tlac_door_locked_back;
    tlinfo.action_data[Lib_tlog::tlog_action_data_0] = Lib_tlog::tlog_data_0_door_open_schedule_end;
    tlinfo.channel = Lib_tlog::tlfp_io_channel;
    Event_manager::send_event(tlinfo);
}
/**
 * \brief Action to be taken GPI 0 is triggered
 *
 */
void GPIO::thread_read_thread_level_changes()
{
    if (GPIO1.get_functional_mode() == mode_ttl)
    {
        int32_t gpo_0_state = 0;
        int32_t gpo_1_state = 0;
        int32_t gpo_2_state = 0;

        int32_t gpo_0_state_new = 0;
        int32_t gpo_1_state_new = 0;
        int32_t gpo_2_state_new = 0;

        get_gpi_state(gpo_0_state_new, gpo_1_state_new, gpo_2_state_new);

        gpo_0_state = gpo_0_state_new;
        gpo_1_state = gpo_1_state_new;
        gpo_2_state = gpo_2_state_new;


        while(1)
        {
            if (GPIO1.get_functional_mode() != mode_ttl)
            {
                break;
            }
            get_gpi_state(gpo_0_state_new, gpo_1_state_new, gpo_2_state_new);

            if ((gpo_0_state != gpo_0_state_new) ||
                    (gpo_1_state != gpo_1_state_new) ||
                    (gpo_2_state != gpo_2_state_new))
            {
                std::string debug = "GPI TL change";
                this->call_tl_setting_changed_callback(debug);
            }

            gpo_0_state = gpo_0_state_new;
            gpo_1_state = gpo_1_state_new;
            gpo_2_state = gpo_2_state_new;

            sleep(1);
        }
    }
}

/**
 * \brief Action to be taken GPI 0 is triggered
 *
 */
void GPIO::handle_gpi_0_triggered()
{
    cout << "In handle GPI 0 triggered \n";

    if (GPIO1.get_functional_mode() == mode_gpio)
    {
        int32_t gpi_0_table = 0;

        int32_t retval;

        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_TABLE_GPI_0, &gpi_0_table);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi_0_table, return: %d", retval);
            return;
        }

        if (gpi_action_delete == gpi_0_table)
        {
            MA5G_firmware::cmd_delete_all_users(Dist_cmd_processor::Comm_channel::unknown);
        }
        else if (gpi_action_reboot_processor == gpi_0_table)
        {
            MA5G_firmware::terminal_reboot(Dist_cmd_processor::Comm_channel::unknown);
        }
        else if (gpi_action_generate_alarm == gpi_0_table)
        {
//            Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
            MA5G_firmware::p_dbus_nonblock->send_gui_signal_play_gpi_alarm();
        }
    }
}

/**
 * \brief Action to be taken GPI 1 is triggered
 *
 */
void GPIO::handle_gpi_1_triggered()
{
    cout << "In handle GPI 1 triggered \n";

    if (GPIO1.get_functional_mode() == mode_gpio)
    {
        int32_t gpi_1_table = 0;

        int32_t retval;

        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_TABLE_GPI_1, &gpi_1_table);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi_1_table, return: %d", retval);
            return;
        }

        if (gpi_action_delete == gpi_1_table)
        {
            MA5G_firmware::cmd_delete_all_users(Dist_cmd_processor::Comm_channel::unknown);
        }
        else if (gpi_action_reboot_processor == gpi_1_table)
        {
            MA5G_firmware::terminal_reboot(Dist_cmd_processor::Comm_channel::unknown);
        }
        else if (gpi_action_generate_alarm == gpi_1_table)
        {
//            Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
            MA5G_firmware::p_dbus_nonblock->send_gui_signal_play_gpi_alarm();
        }
    }
}

/**
 * \brief Action to be taken GPI 2 is triggered
 *
 */
void GPIO::handle_gpi_2_triggered()
{
    cout << "In handle GPI 2 triggered \n";

    if (GPIO1.get_functional_mode() == mode_gpio)
    {
        int32_t gpi_2_table = 0;

        int32_t retval;

        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_TABLE_GPI_2, &gpi_2_table);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi_2_table, return: %d", retval);
            return;
        }

        if (gpi_action_delete == gpi_2_table)
        {
            MA5G_firmware::cmd_delete_all_users(Dist_cmd_processor::Comm_channel::unknown);
        }
        else if (gpi_action_reboot_processor == gpi_2_table)
        {
            MA5G_firmware::terminal_reboot(Dist_cmd_processor::Comm_channel::unknown);
        }
        else if (gpi_action_generate_alarm == gpi_2_table)
        {
//            Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
            MA5G_firmware::p_dbus_nonblock->send_gui_signal_play_gpi_alarm();
        }
    }
}

/**
 * configure default gpo state
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_default_gpo_state(std::string &key_str)
{
	int32_t gpo_0_state = 0;
	int32_t gpo_1_state = 0;
	int32_t gpo_2_state = 0;

	get_default_gpo_state(gpo_0_state, gpo_1_state, gpo_2_state);

	if (0 == key_str.compare(P_GPIO_GPO_0_DEFAULT_STATE))
	{
	    GPIO1.gpio_set_gpo_default_state(gpo0_index, (GPIO_state) gpo_0_state);
	}
	if (0 == key_str.compare(P_GPIO_GPO_1_DEFAULT_STATE))
	{
	    GPIO1.gpio_set_gpo_default_state(gpo1_index, (GPIO_state) gpo_1_state);
	}
	if (0 == key_str.compare(P_GPIO_GPO_2_DEFAULT_STATE))
	{
	    GPIO1.gpio_set_gpo_default_state(gpo2_index, (GPIO_state) gpo_2_state);
	}

}

/**
 * \brief
 *
 * \param line
 * \param state
 * \param duration
 */
void GPIO::set_gpo_state(int32_t line, int32_t state, int32_t duration)
{
    GPIO1.gpio_set_gpo_state((GPO_pin_index) line, (GPIO_state) state, duration);
}

/**
 * \brief
 *
 * \param gpo_0_state
 * \param gpo_1_state
 * \param gpo_2_state
 */
void GPIO::get_gpo_state(int32_t& gpo_0_state, int32_t& gpo_1_state, int32_t& gpo_2_state)
{
    GPIO1.gpio_get_gpo_state(gpo0_index, (GPIO_state*) &gpo_0_state);
    GPIO1.gpio_get_gpo_state(gpo1_index, (GPIO_state*) &gpo_1_state);
    GPIO1.gpio_get_gpo_state(gpo2_index, (GPIO_state*) &gpo_2_state);
}

/**
 * \brief
 *
 * \param line
 */
void GPIO::reset_gpo_state(int32_t line)
{
    int32_t gpo_0_state = 0;
    int32_t gpo_1_state = 0;
    int32_t gpo_2_state = 0;

    get_default_gpo_state(gpo_0_state, gpo_1_state, gpo_2_state);

    if (line == 0)
    {
        set_gpo_state(0, gpo_0_state, 0);
    }
    else if (line == 1)
    {
        set_gpo_state(1, gpo_1_state, 0);
    }
    else if (line == 2)
    {
        set_gpo_state(2, gpo_2_state, 0);
    }
    else
    {
        throw Invalid_parameter_value("Invalid parameter", __PRETTY_FUNCTION__, __LINE__, "GPO line", 0);
    }

}

/**
 * gets GPI line state
 * \param [out] gpo_0_state GPO 0 line state
 * \param [out] gpo_1_state GPO 1 line state
 * \param [out] gpo_2_state GPO 2 line state
 */
void GPIO::get_gpi_state(int32_t& gpo_0_state, int32_t& gpo_1_state, int32_t& gpo_2_state)
{

    GPIO1.gpio_get_gpi_state(gpi0_index, (GPIO_state*) &gpo_0_state);
    GPIO1.gpio_get_gpi_state(gpi1_index, (GPIO_state*) &gpo_1_state);
    GPIO1.gpio_get_gpi_state(gpi2_index, (GPIO_state*) &gpo_2_state);

}

/**
 * configures GPO table
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_gpo_table(std::string &key_str)
{
	int32_t gpo_0_state = 0;
	int32_t gpo_1_state = 0;
	int32_t gpo_0_1_state = 0;
	int32_t gpo_2_state = 0;

    int32_t retval;

    if (0 == key_str.compare(P_GPIO_GPO_TABLE_GPO_0))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_0, &gpo_0_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 0, return: %d", retval);
            return;
        }

        m_gpo_table.gpo_0_state = gpo_0_state;
    }

    if (0 == key_str.compare(P_GPIO_GPO_TABLE_GPO_1))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_1, &gpo_1_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 1, return: %d", retval);
            return;
        }

        m_gpo_table.gpo_1_state = gpo_1_state;
    }

    if (0 == key_str.compare(P_GPIO_GPO_TABLE_GPO_0_AND_1))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_0_AND_1, &gpo_0_1_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 0 and 1, return: %d", retval);
            return;
        }

        m_gpo_table.gpo_0_1_state = gpo_0_1_state;
    }

    if (0 == key_str.compare(P_GPIO_GPO_TABLE_GPO_2))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_2, &gpo_2_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 2, return: %d", retval);
            return;
        }

        m_gpo_table.gpo_2_state = gpo_2_state;
    }

}

/**
 * configures GPI table
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_gpi_table(std::string &key_str)
{
	int32_t gpi_0_state = 0;
	int32_t gpi_1_state = 0;
	int32_t gpi_2_state = 0;

    int32_t retval;

    if (0 == key_str.compare(P_GPIO_GPI_TABLE_GPI_0))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_TABLE_GPI_0, &gpi_0_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi table gpi 0, return: %d", retval);
            return;
        }

        m_gpi_table.gpi_0_state = gpi_0_state;
    }

    if (0 == key_str.compare(P_GPIO_GPI_TABLE_GPI_1))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_TABLE_GPI_1, &gpi_1_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi table gpi 1, return: %d", retval);
            return;
        }

        m_gpi_table.gpi_1_state = gpi_1_state;
    }

    if (0 == key_str.compare(P_GPIO_GPI_TABLE_GPI_2))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_TABLE_GPI_2, &gpi_2_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi table gpi 2, return: %d", retval);
            return;
        }

        m_gpi_table.gpi_2_state = gpi_2_state;
    }

}

/**
 * configures the GPI duration
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_gpi_duration(std::string &key_str)
{
	int32_t gpi_duration;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_DURATION, &gpi_duration);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi duration, return: %d", retval);
        return;
    }


    GPIO1.gpio_set_gpi_trig_duration(GPIO_library::gpi0_index, gpi_duration);
    GPIO1.gpio_set_gpi_trig_duration(GPIO_library::gpi1_index, gpi_duration);
    GPIO1.gpio_set_gpi_trig_duration(GPIO_library::gpi2_index, gpi_duration);
}

/**
 * configures GPI active threat level
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_gpi_active_level(std::string &key_str)
{
	int32_t gpi_active_level;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPI_ACTIVE_LEVEL, &gpi_active_level);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi duration, return: %d", retval);
        return;
    }


    GPIO1.gpio_set_gpi_active_level(GPIO_library::gpi0_index, (GPIO_state)gpi_active_level);
    GPIO1.gpio_set_gpi_active_level(GPIO_library::gpi1_index, (GPIO_state)gpi_active_level);
    GPIO1.gpio_set_gpi_active_level(GPIO_library::gpi2_index, (GPIO_state)gpi_active_level);
}

/**
 * \brief Validate door open schedule mode
 *
 * \param[in] key Key whose validation need to be done
 * \param[in] var Variant to hold the value
 * \returns
 */
int32_t GPIO::validate_door_open_schedule_mode(const std::string& key,const Distant_cmd::Variant& var)
{
    int32_t door_open_schedule_mode = var.int32_value ;
    int32_t gpio_func_mode = 0 ;
    int32_t retval = Database_helper::get_val_from_db((char*) P_GPIO_FUNC_MODE, &gpio_func_mode);
    if (SUCCESS == retval)
    {
        if(gpio_func_mode != 2 && door_open_schedule_mode == 1)
        {
            throw Invalid_parameter_value("Cannot set door open schedule mode to SDAC mode is disabled", __PRETTY_FUNCTION__, __LINE__, "door_open_schedule mode", 0);
        }

    }

    return SUCCESS;

}

/**
 * \brief Validate GPIO mode
 *
 * \param[in] key Key whose validation need to be done
 * \param[in] var Variant to hold the value
 * \returns
 */
int32_t GPIO::validate_gpio_mode(const std::string& key,const Distant_cmd::Variant& var)
{
    int32_t func_mode = var.int32_value;

    if (mode_sdac == func_mode)
    {
        if (false == License_tools::is_sdac_license_present())
        {
            throw License_not_present("Cannot set SDAC mode as SDAC license is not present", __PRETTY_FUNCTION__, __LINE__, "GPO mode", 0);
        }
    }

    int32_t threat_level_mode = 0;

    int32_t retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_MODE, &threat_level_mode);

    if (SUCCESS == retval)
    {
        if (UCC_TL_mode::command_based == threat_level_mode && mode_ttl == func_mode)
        {
            throw Invalid_parameter_value("Cannot set gpio functional mode to TTL as cmd based threat level is active", __PRETTY_FUNCTION__, __LINE__, "GPO mode", 0);
        }
        else if (UCC_TL_mode::TTL_based == threat_level_mode && (mode_gpio == func_mode || mode_sdac == func_mode))
        {
            throw Invalid_parameter_value("Cannot set gpio functional mode to General GPIO/SDAC as GPIO based threat level is active", __PRETTY_FUNCTION__, __LINE__, "GPO mode", 0);
        }
        else if (UCC_TL_mode::disabled == threat_level_mode && mode_ttl == func_mode)
        {
            throw Invalid_parameter_value("Cannot set gpio functional mode to TTL as threat level mode is set to none", __PRETTY_FUNCTION__, __LINE__, "GPO mode", 0);
        }
    }

    return SUCCESS;
}

/**
 * \brief
 *
 */
void GPIO::configure_gpio_mode(std::string &key_str)
{
	int32_t mode;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_FUNC_MODE, &mode);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpio mode, return: %d", retval);
        return;
    }

    if (GPIO1.get_functional_mode() == mode)
    {
    	return;
    }

    if (mode_sdac != mode)
    {
    	m_door_held_open_status = false;
		m_door_forced_open_status = false;

        boost::thread thread_hide_fw_message = boost::thread(&GPIO::hide_fw_message_signal, this);
        boost::thread thread_stop_audio = boost::thread(&GPIO::stop_audio_signal, this);
        stop_tom(true);

        //Disabling door open schedule
		int32_t door_open_schedule_mode = 0;

		int32_t retval = Database_helper::set_val_in_db(P_GPIO_DOOR_OPEN_SCHEDULE, &door_open_schedule_mode, db_unsigned_int, sizeof(uint32_t));

		if (SUCCESS != retval)
		{
			MA5G_logger::log(PRIORITY_ERROR, "Failed to set door open schedule, return: %d", retval);
			return;
		}

        sdac_door_open_schedule_set(false);
    }
    else
    {

    	GPIO1.set_functional_mode(mode_gpio);

    	GPIO1.gpio_set_gpo_state(gpo0_index, gpio_state_low, 0);
		GPIO1.gpio_set_gpo_state(gpo1_index, gpio_state_low, 0);
		GPIO1.gpio_set_gpo_state(gpo2_index, gpio_state_low, 0);

    	GPIO1.gpio_set_gpo_default_state(gpo0_index, gpio_state_high);
		GPIO1.gpio_set_gpo_default_state(gpo1_index, gpio_state_high);
		GPIO1.gpio_set_gpo_default_state(gpo2_index, gpio_state_high);

    }


    GPIO1.set_functional_mode((Functional_mode) mode);


    if (mode_sdac == mode)
    {
    	std::string str;

    	configure_door_controller_params(str);

    	configure_relay_default_state(str);
    }

    boost::thread thread_event_processor = boost::thread(&GPIO::SDAC_GPIO_event_processor, this);
    thread_event_processor.detach();


    if (mode_gpio == mode || mode_ttl == mode)
    {
        std::string key_str_2 = "";

        configure_gpi_active_level(key_str_2);


        std::string key_str = std::string(P_GPIO_GPO_0_DEFAULT_STATE);
        configure_default_gpo_state(key_str);

        key_str = std::string(P_GPIO_GPO_1_DEFAULT_STATE);
        configure_default_gpo_state(key_str);

        key_str = std::string(P_GPIO_GPO_2_DEFAULT_STATE);
        configure_default_gpo_state(key_str);
    }

    // GPIO Mode change, now call TL setting change callback
    MA5G_logger::log(PRIORITY_DEBUG, "GPIO function mode changed to=%d", mode);
    std::string debug = "GPIO function mode change";
    this->call_tl_setting_changed_callback(debug);

    if (mode_ttl == mode)
    {
        //Start thread for thread level
        MA5G_logger::log(PRIORITY_DEBUG, "**** Starting GPIO TTL thread *** \n");
        thread_thread_level = boost::thread(&GPIO::thread_read_thread_level_changes, this);
    }
}

/**
 * \brief Send hide fw message signal in thread
 *
 */
void GPIO::hide_fw_message_signal()
{
    boost::mutex::scoped_lock lock(gpio_mutex);
//    Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
    MA5G_firmware::p_dbus_nonblock->gui_hide_alert_message();
}

void GPIO::stop_audio_signal()
{
    boost::mutex::scoped_lock lock(gpio_mutex);
//    Dbus_call *dbus_obj = Dbus_call::get_msg_dbus_obj();
    MA5G_firmware::p_dbus_nonblock->send_gui_signal_stop_audio();
}

/**
 * \brief Unlock door
 *
 */
int32_t GPIO::sdac_unlock_door()
{
    if (0 == m_door_open_schedule_door_status && sdac_tom_disable == GPIO1.sdac_is_tom_running())
    {
        std:string key_str;
        configure_door_controller_params(key_str);

        return GPIO1.sdac_door_unlock();
    }
    else
    {
        return 0;
    }
}

//! \brief Unlock door with user unlock duration
int32_t GPIO::sdac_unlock_door(int32_t user_unlock_dur)
{
    SDAC_config_param_t sdac_config;

    GPIO1.sdac_get_config(sdac_config);

    if (user_unlock_dur > 0)
    {
        sdac_config.sdc_door_unlock_dur = user_unlock_dur;
        GPIO1.sdac_set_config(sdac_config);
    }
    else
    {
        std:string key_str;
        configure_door_controller_params(key_str);
    }


    if (0 == m_door_open_schedule_door_status && sdac_tom_disable == GPIO1.sdac_is_tom_running())
    {
        return GPIO1.sdac_door_unlock();
    }
    else
    {
        return 0;
    }
}

/**
 * \brief Enabled/Disable door open schedule
 *
 * \param enable
 */
int32_t GPIO::sdac_door_open_schedule_set(bool enable)
{
	return GPIO1.sdac_door_open_schedule_set(enable);
}

/**
 * Get the TOM mode (Enable/Disable) value from device
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_tom_mode(std::string &key_str)
{
    if (GPIO1.get_functional_mode() != mode_sdac)
    {
        //throw GPIO_invalid_mode_for_function("SDAC mode not activate so cant set TOM mode", __PRETTY_FUNCTION__, __LINE__, "", 0);
        MA5G_logger::log(PRIORITY_ERROR, "SDAC mode not active so cant set TOM mode");
    }
    else
    {
        int32_t tom_mode;

        int32_t retval;

        retval = Database_helper::get_val_from_db((char*) P_GPIO_TOM_MODE, &tom_mode);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom mode, return: %d", retval);
            return;
        }

        if (tom_mode == sdac_tom_disable)
        {
            stop_tom(true);
        }

        if(MA5G_firmware::is_MA5G_protocol() &&
        		tom_mode == sdac_tom_enable)
        {
            if (GPIO1.get_functional_mode() != mode_sdac)
            {
                MA5G_logger::log(PRIORITY_ERROR, "SDAC mode not activate so cant start TOM");
        	    int32_t tom_mode = 0;
        	    retval = Database_helper::set_val_in_db(P_GPIO_TOM_MODE, &tom_mode, db_unsigned_int, sizeof(uint32_t));

        	    if (SUCCESS != retval)
        	    {
        	        MA5G_logger::log(PRIORITY_ERROR, "Failed to set tom mode in database, return: %d", retval);
        	        return;
        	    }

            }
            else
            {
            	start_tom();
            }
        }

    }

}


/**
 * Get the duration value for TOM mode
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_tom_dur(std::string &key_str)
{
    if (GPIO1.get_functional_mode() != mode_sdac)
    {
        //throw GPIO_invalid_mode_for_function("SDAC mode not activate so cant set TOM duration", __PRETTY_FUNCTION__, __LINE__, "", 0);
        MA5G_logger::log(PRIORITY_ERROR, "SDAC mode not activate so cant set TOM duration");
    }
    else
    {

        int32_t retval;

        int32_t tom_mode;

        retval = Database_helper::get_val_from_db((char*) P_GPIO_TOM_MODE, &tom_mode);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom mode, return: %d", retval);
            return;
        }

        if(MA5G_firmware::is_bioscrypt_protocol() &&
        		tom_mode == sdac_tom_enable)
		{
			start_tom();
		}

    }

}

/**
 * \brief Return status of whether TOM is running
 *
 */
int32_t GPIO::is_tom_running()
{
    return GPIO1.sdac_is_tom_running();
}

/**
 * Sets the parameters for SDAC
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_door_controller_params(std::string &key_str)
{
    SDAC_config_param_t sdac_config;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_SDAC_DOOR_UNLOCK_DUR, (int32_t*)&sdac_config.sdc_door_unlock_dur);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom duration, return: %d", retval);
        return;
    }

    retval = Database_helper::get_val_from_db((char*) P_GPIO_SDAC_MAX_DOOR_HELD_OPEN_DUR, (int32_t*)&sdac_config.sdc_max_hldopen_dur);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom duration, return: %d", retval);
        return;
    }

    retval = Database_helper::get_val_from_db((char*) P_GPIO_SDAC_RTE_MODE, (int32_t*)&sdac_config.sdc_req_exit_mode);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom duration, return: %d", retval);
        return;
    }

    retval = Database_helper::get_val_from_db((char*) P_GPIO_SDAC_RTE_EGRESS_TIMEOUT, (int32_t*)&sdac_config.sdc_motion_egress_tmout);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom duration, return: %d", retval);
        return;
    }

    retval = Database_helper::get_val_from_db((char*) P_GPIO_SDAC_PUSH_BTN_MODE, (int32_t*)&sdac_config.sdc_push_button_mode);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get tom duration, return: %d", retval);
        return;
    }

    GPIO1.sdac_set_config(sdac_config);
}

/**
 * \brief Configures biometric schedule
 *
 * \param key_str
 */
void GPIO::configure_biometric_schedule(std::string &key_str)
{

	int32_t retval;

	retval = Database_helper::get_val_from_db((char *) P_BIOMETRIC_SCHEDULE, (int32_t*)m_schedule_data[0]);

	if (SUCCESS != retval)
	{
		MA5G_logger::log(PRIORITY_ERROR, "Failed to get biometric schedule, return: %d", retval);
		return;
	}

	process_door_open_schedule();

}

/**
 * \brief Configures the relay default state
 *
 * \param key_str
 */
void GPIO::configure_relay_default_state(std::string &key_str)
{
    int32_t relay_default_state;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_SDAC_RELAY_DEFAULT_STATE, (int32_t*)&relay_default_state);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get sdac relay default state, return: %d", retval);
        return;
    }

    GPIO1.sdac_set_relay_default_state(relay_default_state);
}

/**
 * \brief
 *
 * \param status
 */
void GPIO::get_door_open_status(int32_t& status)
{
    status = GPIO1.sdac_get_door_status();
}

/**
 * \brief Get SDAC status
 *
 * \param relay_state
 * \param door_status
 * \param req_to_exit_state
 */
void GPIO::get_sdac_status(int32_t& relay_state, int32_t& door_status, int32_t& req_to_exit_state)
{
    if ((GPIO1.sdac_get_relay_state() == 1) || (is_tom_running() == sdac_tom_enable) || (1 == m_door_open_schedule_door_status))
    {
        relay_state = 1;
    }
    else
    {
        relay_state = 0;
    }

    door_status = GPIO1.sdac_get_door_status();
    req_to_exit_state = GPIO1.sdac_get_req_to_exit_state();
}

/**
 * configure mode of device threat level
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_device_threat_level_mode(std::string &key_str)
{
	int32_t ttl_mode;
	int32_t gpio_func_mode;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_MODE, &ttl_mode);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get device threat level mode, return: %d", retval);
        return;
    }


    retval = Database_helper::get_val_from_db((char*) P_GPIO_FUNC_MODE, &gpio_func_mode);

    if (SUCCESS == retval)
    {

        if (UCC_TL_mode::TTL_based == ttl_mode && gpio_func_mode != mode_ttl)
        {
            gpio_func_mode = mode_ttl;
            retval = Database_helper::set_val_in_db(P_GPIO_FUNC_MODE, &gpio_func_mode, db_unsigned_int, sizeof(uint32_t));
        }
        else if ((UCC_TL_mode::command_based == ttl_mode || UCC_TL_mode::disabled == ttl_mode) && gpio_func_mode == mode_ttl)
        {
            gpio_func_mode = mode_gpio;
            retval = Database_helper::set_val_in_db(P_GPIO_FUNC_MODE, &gpio_func_mode, db_unsigned_int, sizeof(uint32_t));
        }

    }

    // Update per user rule, for L1 mode only
    if ( MA5G_firmware::is_bioscrypt_protocol())
    {
        MA5G_firmware::user_control_update_L1_TL_workflow_settting();
    }


    // Threat level mode change, now call TL setting change callback
    MA5G_logger::log(PRIORITY_DEBUG, "TL mode=%d", ttl_mode);
    std::string debug = "TL mode changed";
    this->call_tl_setting_changed_callback(debug);
}


/**
 * configures command based active threat level
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_command_based_active_threat_level(std::string &key_str)
{
    int32_t level;

    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_CMD_BASED_ACTIVE_THREAT_LEVEL, &level);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get cmd based active threat level, return: %d", retval);
        return;
    }

    // Command based active threat level change, now call TL setting change callback
    MA5G_logger::log(PRIORITY_DEBUG, "command based TL=%d", level);
    std::string debug = "command based TL change";
    this->call_tl_setting_changed_callback(debug);
}


/**
 * \brief
 *
 * \param threat_level_mode
 * \param active_threat_level
 */
void GPIO::get_current_threat_level(int32_t& threat_level_mode, int32_t& active_threat_level)
{
    int32_t retval;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_MODE, &threat_level_mode);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get threat level mode, return: %d", retval);
        return;
    }


    if(UCC_TL_mode::TTL_based == threat_level_mode)
    {
        int32_t gpo_0_state = 0;
        int32_t gpo_1_state = 0;
        int32_t gpo_2_state = 0;

        get_gpi_state(gpo_0_state, gpo_1_state, gpo_2_state);

        if(GPIO_library::gpio_state_low == gpo_0_state &&
                GPIO_library::gpio_state_low == gpo_1_state)
        {
            retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_0, &active_threat_level);

            if (SUCCESS != retval)
            {
                MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi 0 ttl mapping, return: %d", retval);
                return;
            }

        }
        else if(GPIO_library::gpio_state_high == gpo_0_state &&
                GPIO_library::gpio_state_high == gpo_1_state)
        {
            retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_3, &active_threat_level);

            if (SUCCESS != retval)
            {
                MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi 0 ttl mapping, return: %d", retval);
                return;
            }
        }
        else if(GPIO_library::gpio_state_low == gpo_0_state &&
                GPIO_library::gpio_state_high == gpo_1_state)
        {
            retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_2, &active_threat_level);

            if (SUCCESS != retval)
            {
                MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi 0 ttl mapping, return: %d", retval);
                return;
            }
        }
        else if(GPIO_library::gpio_state_high == gpo_0_state &&
                GPIO_library::gpio_state_low == gpo_1_state)
        {
            retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_1, &active_threat_level);

            if (SUCCESS != retval)
            {
                MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpi 0 ttl mapping, return: %d", retval);
                return;
            }
        }

    }
    else
    {

        retval = Database_helper::get_val_from_db((char*) P_GPIO_CMD_BASED_ACTIVE_THREAT_LEVEL, &active_threat_level);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get cmd based active threat level, return: %d", retval);
            return;
        }

    }

}


/**
 * configure access rule priority
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_access_rule_priority(std::string &key_str)
{
    // Update per user rule, for L1 mode only
    if ( MA5G_firmware::is_bioscrypt_protocol())
    {
        // Update per user rule if TL is also enabled
        MA5G_firmware::user_control_update_L1_TL_workflow_settting();
    }
}

/**
 * Receives distant command data to get door open schedule and passes the information to Command_handler class
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_door_open_schedule(std::string &key_str)
{
	int32_t mode;

	int32_t retval;

	retval = Database_helper::get_val_from_db((char*) P_GPIO_DOOR_OPEN_SCHEDULE, &mode);

	if (SUCCESS != retval)
	{
		MA5G_logger::log(PRIORITY_ERROR, "Failed to get door open schedule, return: %d", retval);
		return;
	}

	m_door_open_schedule_mode = mode;

	process_door_open_schedule();
}

/**
 * configure GPI to its threat level map
 * \param key_str database key for the handler for which the API is called
 */
void GPIO::configure_gpi_to_threat_level_mapping(std::string &key_str)
{
	int32_t gpi0_threat_level = 0;
	int32_t gpi1_threat_level = 0;
	int32_t gpi2_threat_level = 0;
	int32_t gpi3_threat_level = 0;

    int32_t retval;

    if (0 == key_str.compare(P_GPIO_THREAT_LEVEL_GPI_0))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_0, &gpi0_threat_level);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpio threat level gpi 0, return: %d", retval);
            return;
        }
    }

    if (0 == key_str.compare(P_GPIO_THREAT_LEVEL_GPI_1))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_1, &gpi1_threat_level);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpio threat level gpi 1, return: %d", retval);
            return;
        }
    }

    if (0 == key_str.compare(P_GPIO_THREAT_LEVEL_GPI_2))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_2, &gpi2_threat_level);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpio threat level gpi 2, return: %d", retval);
            return;
        }
    }

    if (0 == key_str.compare(P_GPIO_THREAT_LEVEL_GPI_3))
    {
        retval = Database_helper::get_val_from_db((char*) P_GPIO_THREAT_LEVEL_GPI_3, &gpi3_threat_level);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpio threat level gpi 3, return: %d", retval);
            return;
        }
    }

    std::string debug = "GPI to TL mapping change";
    // GPI to TL mapping change, now call TL setting change callback
    this->call_tl_setting_changed_callback(debug);
}

/**
 * Process the event and toggles the GPO state as per configuration in database
 * \param event transaction log data
 */
void GPIO::process_event_gpo(const Tlog_info& event)
{
    int32_t gpo_0_toggle = 0;
    int32_t gpo_1_toggle = 0;
    int32_t gpo_2_toggle = 0;

    if(MA5G_firmware::is_MA5G_protocol())
    {
        gpo_0_toggle = Event_manager::event_settings_table[event.action].trigger_TTL0;
        gpo_1_toggle = Event_manager::event_settings_table[event.action].trigger_TTL1;
        gpo_2_toggle = Event_manager::event_settings_table[event.action].trigger_TTL2;
    }
    else if(MA5G_firmware::is_bioscrypt_protocol())
    {

        int32_t gpo_table_action = 0;

        if (Lib_tlog::tlac_user_control_successful == event.action)
        {
            gpo_table_action = GP0_ACTION_VERIFY_PASS;
        }
        else if (Lib_tlog::tlac_control_timed_out == event.action &&
        		event.action_data[Lib_tlog::tlog_action_data_2] == Lib_tlog::tlog_data_2_bio_not_detected)
        {
            gpo_table_action = GP0_ACTION_FINGER_NOT_DETECTED;
        }
        else if (Lib_tlog::tlac_biometric_mismatch == event.action ||
                 Lib_tlog::tlac_pin_mismatch == event.action ||
                 Lib_tlog::tlac_user_id_not_in_db == event.action ||
                 Lib_tlog::tlac_control_timed_out == event.action ||
                 Lib_tlog::tlac_rejected_by_schedule == event.action ||
                 Lib_tlog::tlac_temp_validity_expired == event.action ||
                 Lib_tlog::tlac_usr_rule_check_failure == event.action ||
                 Lib_tlog::tlac_user_not_in_white_list == event.action ||
                 Lib_tlog::tlac_black_listed_card == event.action ||
                 Lib_tlog::tlac_face_not_detected == event.action )

        {
            gpo_table_action = GP0_ACTION_VERIFY_FAIL;
        }
        else if (Lib_tlog::tlac_management_menu_login == event.action &&
        			event.role == Lib_tlog::user_full_admin)
        {
            gpo_table_action = GP0_ACTION_ADMIN_MODE;
        }
        else if (Lib_tlog::tlac_management_menu_login == event.action &&
    			event.role == Lib_tlog::user_db_admin)
        {
            gpo_table_action = GP0_ACTION_ENROLL_MODE;
        }
        else if (Lib_tlog::tlac_user_deleted == event.action ||
        			Lib_tlog::tlac_database_deleted == event.action)
        {
            gpo_table_action = GP0_ACTION_DELETE_ATTEMPTED;
        }
        else if (Lib_tlog::tlac_terminal_boot_completed == event.action)
        {
            gpo_table_action = GP0_ACTION_PROCESSOR_BOOTUP;
        }
        /*else if (Lib_tlog::tlac_file_transfer_complete == event.action)
        {
            gpo_table_action = GP0_ACTION_FILE_TRANSFER_COMPLETE;
        }*/
        else if (Lib_tlog::tlac_tamper_detected == event.action)
        {
            gpo_table_action = GP0_ACTION_TAMPER_DETECTED;
        }
        else if (Lib_tlog::tlac_duress_finger_detected == event.action)
        {
            gpo_table_action = GP0_ACTION_DURESS_FINGER_DETECTED;
        }
        else
        {
            return;
        }

        int32_t gpo_0_state = 0;
        int32_t gpo_1_state = 0;
        int32_t gpo_2_state = 0;

        int32_t retval = 0;

        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_0, &gpo_0_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 0 value, return: %d",
                    retval);
            return;
        }

        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_1, &gpo_1_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 1 value, return: %d",
                    retval);
            return;
        }

        retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_TABLE_GPO_2, &gpo_2_state);

        if (SUCCESS != retval)
        {
            MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo table gpo 2 value, return: %d",
                    retval);
            return;
        }


        for(int32_t i=0; i<=12; ++i)
        {
            int32_t byte_action_gpo0 = 0;
            int32_t byte_action_gpo1 = 0;
            int32_t byte_action_gpo2 = 0;

            byte_action_gpo0 = (gpo_0_state & (0x01<<i)) >> (i);
            byte_action_gpo1 = (gpo_1_state & (0x01<<i)) >> (i);
            byte_action_gpo2 = (gpo_2_state & (0x01<<i)) >> (i);

            if (i == gpo_table_action)
            {
                switch(i)
                {
                case GP0_ACTION_ENROLL_COMPLETED:
                    //printf("******* Enroll comp \n");
                case GP0_ACTION_ENROLL_INITIATED:
                    //printf("******* Enroll initiated \n");
                case GP0_ACTION_VERIFY_PASS:
                    //printf("******* Verify identify passed \n");
                case GP0_ACTION_VERIFY_FAIL:
                    //printf("******* Verify identify failed \n");
                case GP0_ACTION_FINGER_NOT_DETECTED:
                    //printf("******* Finger not detected \n");
                case GP0_ACTION_ADMIN_MODE:
                    //printf("*******Admin mode \n");
                case GP0_ACTION_ENROLL_MODE:
                    //printf("******* Enroll mode \n");
                case GP0_ACTION_DELETE_ATTEMPTED:
                    //printf("******* Delete Attempted \n");
                case GP0_ACTION_PROCESSOR_BOOTUP:
                    //printf("******* Device bootup \n");
                case GP0_ACTION_FILE_TRANSFER_COMPLETE:
                    //printf("******* File transfer complete \n");
                case GP0_ACTION_TAMPER_DETECTED:
                    //printf("******* Tamper occured \n");
                case GP0_ACTION_DURESS_FINGER_DETECTED:
                    //printf("******* Duress finger \n");
                    gpo_0_toggle = (byte_action_gpo0==1)?1:0;
                    gpo_1_toggle = (byte_action_gpo1==1)?1:0;
                    gpo_2_toggle = (byte_action_gpo2==1)?1:0;
                    break;
                }
            }
        }

    }
    else
    {
        return;
    }


    boost::shared_ptr<GPIO> p_gpio = GPIO::get_instance();

    int32_t gpo_0_duration = 0;

    int32_t retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_0_DURATION, &gpo_0_duration);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo 0 duration, return: %d", retval);
        return;
    }


    int32_t gpo_1_duration = 0;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_1_DURATION, &gpo_1_duration);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo 1 duration, return: %d", retval);
        return;
    }


    int32_t gpo_2_duration = 0;

    retval = Database_helper::get_val_from_db((char*) P_GPIO_GPO_2_DURATION, &gpo_2_duration);

    if (SUCCESS != retval)
    {
        MA5G_logger::log(PRIORITY_ERROR, "Failed to get gpo 2 duration, return: %d", retval);
        return;
    }


    int32_t gpo_0_def_state = 0;
    int32_t gpo_1_def_state = 0;
    int32_t gpo_2_def_state = 0;

    p_gpio->get_default_gpo_state(gpo_0_def_state, gpo_1_def_state, gpo_2_def_state);

    gpo_0_def_state = (gpo_0_def_state==0)?1:0;
    gpo_1_def_state = (gpo_1_def_state==0)?1:0;
    gpo_2_def_state = (gpo_2_def_state==0)?1:0;

    if (1 == gpo_0_toggle)
    {
        p_gpio->set_gpo_state(0, gpo_0_def_state, gpo_0_duration);
    }
    if (1 == gpo_1_toggle)
    {
        p_gpio->set_gpo_state(1, gpo_1_def_state, gpo_1_duration);
    }
    if (1 == gpo_2_toggle)
    {
        p_gpio->set_gpo_state(2, gpo_2_def_state, gpo_2_duration);
    }

}

void GPIO::set_threat_level_change_callback(const App_threat_level_setting_change_callback_function &callback)
{
    this->tl_setting_changed_callback = callback;
}


void GPIO::call_tl_setting_changed_callback(const std::string &debug)
{
    if ( this->tl_setting_changed_callback )
    {
        this->tl_setting_changed_callback(debug);
    }
}
