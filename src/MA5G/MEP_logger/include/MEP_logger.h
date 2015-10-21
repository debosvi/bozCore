/// @file MEP_logger.h
/// @brief Logger library public interface
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

#ifndef MEP_LOGGER_H_
#define MEP_LOGGER_H_

#include <log4c.h>

#include <stdint.h>     /* for uint32_t */
#ifdef WIN32
#include <Winsock2.h>   /* for sockaddr_in struct */
#else
#include <netinet/in.h> /* for sockaddr_in struct */
#endif

/**
 * @file MEP_logger.h
 *
 * @brief MEP logger API functions declaration
 *
**/

#define MAX_MEP_LOG_BUFFER	4096

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Alternative log4c initialization
 * @param config_file path and name of the configuration file
 * @return 0 for success
 **/
int log4c_init_with_conf_file(const char* config_file);

/**
 * @brief MEP log library initialization
 * @param config_file path and name of the process logger configuration file
 * @return 0 for success
 **/
int MEP_log_init(const char* config_file);

/**
 * @brief MEP log rolling files delete
 * @param appender name of which the log files are to be deleted
 * @return 0 for success
 **/
int MEP_log_delete(const char* appender_name);

/**
 * @brief MEP formatters initialization
 * @return 0 for success
 **/
int init_MEP_formatters();

/**
 * @brief MEP appenders initialization
 * @return 0 for success
 **/
int init_MEP_appenders();

/**
 * @brief MEP rolling policy initialization
 * @return 0 for success
 **/
int init_MEP_rollingpolicies();

/**
 * @brief MEP log request with the specified priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_priority The priority of this log message
 * @param a_MEP_type The type of MEP log message (text, binary, hexadecimal, none=log4c like)
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 *
 **/
void MEP_log(const log4c_category_t* a_category,
					  int a_priority,
					  const char* a_format,
					  ...);


/**
 * @brief MEP log request with fatal priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_fatal(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request with alert priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_alert(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request with crit priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_crit(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request with error priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_error(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);
/**
 * @brief MEP log request with warn priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_warn(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request with notice priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_notice(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);
/**
 * @brief MEP log request with info priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_info(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request with debug priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_debug(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request with trace priority
 * To configure preferentially with specific MEP layouts (with length, tags, encrypt flag...)
 * @param a_category the log4c_category_t object
 * @param a_format Format specifier for the string to write
 * in the log file.
 * @param ... The arguments for a_format
 **/
void MEP_log_trace(const log4c_category_t* a_category,
					  const char* a_format,
					  ...);

/**
 * @brief MEP log request for binary data with the specified priority.
 * @param a_category the log4c_category_t object
 * @param a_priority The priority of this log message
 * @param a_buffer A buffer pointer on binary data to write
 * in the log file.
 **/
/* void MEP_log_bin(const log4c_category_t* a_category,
					  int a_priority,
					  const void* a_buffer);*/

/**
 * @brief MEP log request for binary data in hexadecimal format with the specified priority.
 * @param a_category the log4c_category_t object
 * @param a_priority The priority of this log message
 * @param a_buffer A buffer pointer on binary data to write
 * @param n_bytes The size of the buffer (in bytes)
 * in the log file.
 **/
void MEP_log_hex(const log4c_category_t* a_category,
					  int a_priority,
					  const void* a_buffer,
					  const size_t n_bytes);

/**
 * @brief MEP dump request for binary buffer in a file at a given location on filesystem
 * @param a_file A file path and name to dump the buffer
 * @param a_buffer A buffer pointer on binary data to write
 * @param n_bytes The size of the buffer (in bytes)
 * @return 0 for success, -1 (opening error), -2 (write error), -3 (close error)
 **/
int MEP_dump(const char* a_file,
					  const void* a_buffer,
					  const size_t n_bytes);

/**
 * @brief MEP dump request for binary buffer on the network
 * @param a_sockaddr A local or remote endpoint address to which to connect a socket
 * @param a_buffer A buffer pointer on binary data to write
 * @param n_bytes The size of the buffer (in bytes)
 * @return 0 for success
 **/
/*int MEP_network_dump(const struct sockaddr_in* psockaddr,
					  const void* a_buffer,
					  const size_t n_bytes);*/

/**
 * @brief MEP log library clean-up
 * @return 0 for success
 **/
int MEP_log_cleanup();


#ifdef __cplusplus
}
#endif

#endif /* MEP_LOGGER_H_ */
