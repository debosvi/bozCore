/**
 * \section LICENSE
 * Copyright &copy; 2013 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 * Implementation file for library of MA5G logger which uses MEP logger
 *
 * \file MA5G_logger.cpp
 */

/****************************************************************
 *   Include Files
 ***************************************************************/
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

#include "Common_inc.h"


/****************************************************************
 *   File Variables
 ***************************************************************/
//Variable which stores the flag of whether the MEP logger library
//has been initialized or not
int32_t MA5G_logger::m_lib_init = 0;
int32_t MA5G_logger::m_logging_enabled = 0;
int32_t MA5G_logger::m_logging_mode = 0;
int32_t MA5G_logger::m_debug_level = 0;
std::string MA5G_logger::m_ip_addr = "";
int32_t MA5G_logger::m_port = 0;

//Object of the MEP logger class
log4c_category_t* MA5G_logger::m_cat_rolling = NULL;
log4c_category_t* MA5G_logger::m_cat_stdout = NULL;
log4c_category_t* MA5G_logger::m_cat_network = NULL;

boost::mutex MA5G_logger::mutex_init;
boost::mutex MA5G_logger::mutex_cleanup;

/****************************************************************
 *   Functions Definition
 ***************************************************************/

/**
 * \brief Constructor
 *
 */
MA5G_logger::MA5G_logger()
{
}

/**
 * \brief Destructor
 *
 */
MA5G_logger::~MA5G_logger()
{
}

int32_t MA5G_logger::sync_conf_file_to_log4crc_file()
{

	int32_t log_enabled = 0;
    int32_t mode = 0;
    int32_t level = 0;
    int32_t port = 0;

    std::string str_ip_addr;
    std::string str_port;

    MA5G_logger::get_conf_from_file(log_enabled, mode, level, str_ip_addr, port);

    std::stringstream ss;
    ss << port;
    str_port = ss.str();

    std::string str_level = "trace";

    switch(level)
    {
    case 0:
        str_level = "fatal";
        break;
    case 1:
        str_level = "alert";
        break;
    case 2:
        str_level = "crit";
        break;
    case 3:
        str_level = "error";
        break;
    case 4:
        str_level = "warn";
        break;
    case 5:
        str_level = "notice";
        break;
    case 6:
        str_level = "info";
        break;
    case 7:
        str_level = "debug";
        break;
    case 8:
        str_level = "trace";
        break;
    case 9:
        str_level = "notset";
        break;
    case 10:
        str_level = "unknown";
        break;
    }

    std::string str_file;
    str_file =  "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
    str_file += "<!DOCTYPE log4c SYSTEM \"\">\n";
    str_file += "\n";
    str_file += "<log4c version=\"1.2.3\">\n";
    str_file += "\n";
    str_file += "   <config>\n";
    str_file += "       <bufsize>0</bufsize>\n";
    str_file += "       <debug level=\"2\"/>\n";
    str_file += "       <nocleanup>0</nocleanup>\n";
    str_file += "       <reread>1</reread>\n";
    str_file += "   </config>\n";
    str_file += "\n";
    str_file += "   <category name=\"MA5Grolling\" priority=\"" + str_level + "\" appender=\"myrollingfileappender\" />\n";
    str_file += "   <category name=\"MA5Grolling_gui\" priority=\"" + str_level + "\" appender=\"myrollingfileappender_gui\" />\n";
    str_file += "   <category name=\"MA5Gstdout\" priority=\"" + str_level + "\" appender=\"stdout\" />\n";
    str_file += "   <category name=\"MA5Gnetwork\" priority=\"" + str_level + "\" appender=\"mysocket\"/>\n";
    str_file += "\n";
    str_file += "   <appender name=\"mysocket\" type=\"MEP_socket\" port=\"" + str_port + "\" ip_address=\"" + str_ip_addr + "\" layout=\"dated\"/>\n";
    str_file += "\n";
    str_file += "   <rollingpolicy name=\"myrollingpolicy\" type=\"complete\" maxsize=\"5242880\" maxnum=\"2\" />\n";
    str_file += "   <rollingpolicy name=\"myrollingpolicy_gui\" type=\"complete\" maxsize=\"5242880\" maxnum=\"2\" />\n";
    str_file += "\n";
#ifdef ARM
    str_file += "   <appender name=\"myrollingfileappender\" type=\"rollingfile\" logdir=\"/rootfs_data/DiagnosticInfo/\" prefix=\"MA5Glog_core\"  layout=\"dated\" rollingpolicy=\"myrollingpolicy\" />\n";
    str_file += "   <appender name=\"myrollingfileappender_gui\" type=\"rollingfile\" logdir=\"/rootfs_data/DiagnosticInfo/\" prefix=\"MA5Glog_gui\"  layout=\"dated\" rollingpolicy=\"myrollingpolicy_gui\" />\n";
#else
    str_file += "   <appender name=\"myrollingfileappender\" type=\"rollingfile\" logdir=\"./data/DiagnosticInfo/\" prefix=\"MA5Glog_core\"  layout=\"dated\" rollingpolicy=\"myrollingpolicy\" />\n";
    str_file += "   <appender name=\"myrollingfileappender_gui\" type=\"rollingfile\" logdir=\"./data/DiagnosticInfo/\" prefix=\"MA5Glog_gui\"  layout=\"dated\" rollingpolicy=\"myrollingpolicy_gui\" />\n";
#endif //#ifdef ARM


    str_file += "\n";
    str_file += "   <appender name=\"stdout\" type=\"stream\" layout=\"dated\"/>\n";
    str_file += "\n";
    str_file += "</log4c>\n";

    std::ofstream conf_file;
    conf_file.open (File_path::log4crc_file.c_str());

    if (conf_file.is_open())
    {
        conf_file.write(str_file.data(), str_file.length());
        conf_file.close();

        sync();

        return 0;
    }
    else
    {
        return -1;
    }

}

/**
 * \brief set_conf_to_file
 *
 * \return int  0 = success, -1 = error
 */
int32_t MA5G_logger::set_conf_to_file(const int32_t log_enabled, const int32_t mode, const int32_t level,
                                        	const std::string& str_ip_addr, const int32_t port)
{
    typedef struct
    {
    	int32_t enabled;
        int32_t mode;
        int32_t level;
        int32_t ip_addr;
        int32_t port;
    }Logger_conf;

    Logger_conf logger_conf;
    logger_conf.enabled = log_enabled;
    logger_conf.mode    = mode;
    logger_conf.level   = level;
    logger_conf.ip_addr = boost::asio::ip::address::from_string(str_ip_addr).to_v4().to_ulong();
    logger_conf.port    = port;


    std::ofstream conf_file;
    conf_file.open (File_path::error_log_conf_file.c_str());

    if (conf_file.is_open())
    {
        conf_file.write((char*)&logger_conf, sizeof(Logger_conf));
        conf_file.close();

        MA5G_logger::sync_conf_file_to_log4crc_file();

        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * \brief get_conf_from_file
 *
 * \return int  0 = success, -1 = error
 */
int32_t MA5G_logger::get_conf_from_file(int32_t& log_enabled, int32_t& mode, int32_t& level, std::string& str_ip_addr, int32_t& port)
{
    typedef struct
    {
    	int32_t enabled;
        int32_t mode;
        int32_t level;
        int32_t ip_addr;
        int32_t port;
    }Logger_conf;


    Logger_conf logger_conf;

    std::ifstream conf_file;
    conf_file.open (File_path::error_log_conf_file.c_str());

    if (conf_file.is_open())
    {
        conf_file.read((char*)&logger_conf, sizeof(Logger_conf));
        conf_file.close();


        log_enabled = logger_conf.enabled;
        mode = logger_conf.mode;
        level = logger_conf.level;

        try
        {
            boost::asio::ip::address_v4 addr(logger_conf.ip_addr);

            str_ip_addr = addr.to_string();

        }
        catch(std::exception& e)
        {
            str_ip_addr = "";
        }

        port = logger_conf.port;

        return 0;
    }
    else
    {
    	//Default params
        log_enabled = 0;
        mode = 1;
        level = 7;
    	str_ip_addr = "0.0.0.0";
    	port = 0;

    	set_conf_to_file(log_enabled, mode, level, str_ip_addr, port);

        return -1;
    }
}

/**
 * \brief Initialize the MEP logger object.
 *        Will check the flag and call the init function of MEP logger only if uninitialized
 *
 * \return int  0 = success, -1 = error
 */
int32_t MA5G_logger::init(bool is_gui)
{
	boost::mutex::scoped_lock lock(mutex_init);

    //Check if MEP logger library is already initialized
    if (1 == m_lib_init)
    {
        return 0;
    }

    //MA5G_logger::set_conf_to_file(0, 7, "0.0.0.0", 0);

    if (MA5G_logger::get_conf_from_file(m_logging_enabled, m_logging_mode, m_debug_level, m_ip_addr, m_port) < 0)
    {
        std::cout << "****** Error in reading logger conf file !!!!!!!!!!!!!! " << std::endl << std::endl;
    }

#ifndef ARM
    m_logging_enabled = 1;
    m_logging_mode = Error_log_mode::std_out;
#endif

    std::cout << "****** m_logging_enabled : " << m_logging_enabled << std::endl;
    std::cout << "****** m_logging_mode : " << m_logging_mode << std::endl;
    std::cout << "****** m_debug_level : " << m_debug_level << std::endl;
    std::cout << "****** m_ip_addr : " << m_ip_addr << std::endl;
    std::cout << "****** m_port : " << m_port << std::endl;


    //Call init function of MEP logger library
    if (MEP_log_init(File_path::log4crc_file.c_str()))
    {
        //Failed
        return -1;
    }
    else
    {
        //Set flag to '1' to inidicate the MEP logger library initialization
        //is successful
        m_lib_init = 1;


        //Assign value to category object as per the configured category name
        //in configuration file
        if (m_logging_mode == Error_log_mode::std_out)
        {
            m_cat_stdout = log4c_category_get("MA5Gstdout");
        }
        else if (m_logging_mode == Error_log_mode::rolling_file)
        {
        	if (is_gui == true)
        	{
        		m_cat_rolling = log4c_category_get("MA5Grolling_gui");
        	}
        	else
        	{
        		m_cat_rolling = log4c_category_get("MA5Grolling");
        	}
        }
        else if (m_logging_mode == Error_log_mode::network_dump)
        {
            m_cat_network = log4c_category_get("MA5Gnetwork");
        }

        return 0;
    }
}

/**
 * \brief Clean the MEP logger object
 *
 * \return int  0 = success, -1 = error
 */
int32_t MA5G_logger::cleanup()
{
	boost::mutex::scoped_lock lock(mutex_cleanup);

    if (0 == m_lib_init)
    {
        return 0;
    }

    //Set flag to '0' to inidicate the MEP logger library is de-initialized
    m_lib_init = 0;

    //Set the category object to NULL
    m_cat_rolling = NULL;
    m_cat_stdout = NULL;
    m_cat_network = NULL;

    //Call cleanup function of MEP logger library
    if (MEP_log_cleanup())
    {
        return -1;
    }

    return 0;
}

/**
 * \brief Insert a log statement in the log file specified in the configuration file.
 *        The 'a_priority' variable is used to specify the priority of the log statement
 *        to be inserted. The 'a_format' contains a plain text, formatted text (%d, %s, etc.)
 *
 * \param a_priority Log message priority from the enum Priority_level_t in header file
 * \param a_format The string in printf type format (with %d, %s, etc. and values)
 *
 */
void MA5G_logger::log(int32_t a_priority, const char* a_format, ...)
{

	if (m_logging_enabled == 0)
	{
		return;
	}


    va_list va;
    va_start(va, a_format);

    //Call the log function of MEP logger library passing all the respective args
    //MEP_log(cat_stdout, a_priority, a_format, va);
    //MEP_log(cat_rolling, a_priority, a_format, va);

    if (m_logging_mode == Error_log_mode::std_out && m_cat_stdout != NULL)
    {
        if (log4c_category_is_priority_enabled(m_cat_stdout, a_priority))
        {
            log4c_category_vlog(m_cat_stdout, a_priority, a_format, va);
        }
    }
    else if (m_logging_mode == Error_log_mode::rolling_file && m_cat_rolling != NULL)
    {
        if (log4c_category_is_priority_enabled(m_cat_rolling, a_priority))
        {
            log4c_category_vlog(m_cat_rolling, a_priority,  a_format, va);
        }
    }
    else if (m_logging_mode == Error_log_mode::network_dump && m_cat_network != NULL)
    {
        if (log4c_category_is_priority_enabled(m_cat_network, a_priority))
        {
            log4c_category_vlog(m_cat_network, a_priority,  a_format, va);
        }
    }

    va_end(va);
}

void MA5G_logger::log_hex(int32_t a_priority, const void* a_buffer, const size_t n_bytes)
{
	if (m_logging_enabled == 0)
	{
		return;
	}

    //Call the MEP_log_hex function of MEP logger library passing all the respective args
    if (m_logging_mode == Error_log_mode::std_out)
    {
        MEP_log_hex(m_cat_stdout, a_priority, a_buffer, n_bytes);
    }
    else if (m_logging_mode == Error_log_mode::rolling_file)
    {
        MEP_log_hex(m_cat_rolling, a_priority, a_buffer, n_bytes);
    }
    else if (m_logging_mode == Error_log_mode::network_dump)
    {
        MEP_log_hex(m_cat_network, a_priority, a_buffer, n_bytes);
    }
}
