/**
 * \section LICENSE
 * Copyright &copy; 2013 Safran Morpho - Safran Morpho confidential - all rights reserved
 *
 * \section DESCRIPTION
 *  Header file for library of MA5G logger which uses MEP logger
 *
 * \file MA5G_logger.h
 */

#ifndef _MA5G_LOGGER_H
#define _MA5G_LOGGER_H

/****************************************************************
 *   Type and Constant Definitions
 ***************************************************************/

/****************************************************************
 *   Include Files
 ***************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <MEP_logger.h>
#include <boost/thread.hpp>

/****************************************************************
 *   Type and Constant Definitions
 ***************************************************************/
/**
 * Predefined Levels of priorities. These correspond to the priority levels
 * used by MEP logger library.
 **/
typedef enum
{
    /** fatal */PRIORITY_FATAL = 000,
    /** alert */PRIORITY_ALERT = 100,
    /** crit */PRIORITY_CRIT = 200,
    /** error */PRIORITY_ERROR = 300,
    /** warn */PRIORITY_WARN = 400,
    /** notice */PRIORITY_NOTICE = 500,
    /** info */PRIORITY_INFO = 600,
    /** debug */PRIORITY_DEBUG = 700,
    /** trace */PRIORITY_TRACE = 800,
    /** notset */PRIORITY_NOTSET = 900,
    /** unknown */PRIORITY_UNKNOWN = 1000
} Priority_level_t;


/**
 * \brief User active TL
 */
struct Error_log_mode
{
    /**
     * enums
     */
    enum type
    {
        std_out,
        rolling_file,
        network_dump
    };
};


/****************************************************************
 *   class Declaration
 ***************************************************************/
class MA5G_logger
{
private:
    static int32_t m_lib_init;
    static int32_t m_logging_enabled;
    static int32_t m_logging_mode;
    static int32_t m_debug_level;
    static std::string m_ip_addr;
    static int32_t m_port;
    static log4c_category_t* m_cat_rolling;
    static log4c_category_t* m_cat_stdout;
    static log4c_category_t* m_cat_network;

    static boost::mutex mutex_init;
    static boost::mutex mutex_cleanup;

public:

    /**
     * \brief Constructor
     *
     */
    MA5G_logger();

    /**
     * \brief Destructor
     *
     */
    ~MA5G_logger();

    /**
     * \brief set_conf_to_file
     *
     * \return int  0 = success, -1 = error
     */
    static int32_t sync_conf_file_to_log4crc_file();

    /**
     * \brief set_conf_to_file
     *
     * \return int  0 = success, -1 = error
     */
    static int32_t set_conf_to_file(const int32_t log_enabled, const int32_t mode, const int32_t level, const std::string& str_ip_addr, const int32_t port);

    /**
     * \brief get_conf_from_file
     *
     * \return int  0 = success, -1 = error
     */
    static int32_t get_conf_from_file(int32_t& log_enabled, int32_t& mode, int32_t& level, std::string& str_ip_addr, int32_t& port);

    /**
     * \brief Initialize the MEP logger object.
     *        Will check the flag and call the init function of MEP logger only if uninitialized
     *
     * \return int  0 = success, -1 = error
     */
    static int32_t init(bool is_gui = false);

    /**
     * \brief Clean the MEP logger object
     *
     * \return int  0 = success, -1 = error
     */
    static int32_t cleanup();

    /**
     * \brief Insert a log statement in the log file specified in the configuration file.
     *        The 'a_priority' variable is used to specify the priority of the log statement
     *        to be inserted. The 'a_format' contains a plain text, formatted text (%d, %s, etc.)
     *
     * \param a_priority Log message priority from the enum Priority_level_t in header file
     * \param a_format The string in printf type format (with %d, %s, etc. and values)
     *
     */
    static void log(int32_t a_priority, const char* a_format, ...) __attribute__ ((format (printf, 2, 3)));

    /**
     * \brief Insert a log statement in the log file specified in the configuration file.
     *        The 'a_priority' variable is used to specify the priority of the log statement
     *        to be inserted. The 'a_format' contains a plain text, formatted text (%d, %s, etc.)
     *
     * \param a_priority Log message priority from the enum Priority_level_t in header file
     * \param a_format The string in printf type format (with %d, %s, etc. and values)
     *
     */
    static void log_hex(int32_t a_priority, const void* a_buffer, const size_t n_bytes);


};

#endif // _MA5G_LOGGER_H
