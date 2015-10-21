/// @file appender_type_socket.h
/// @brief MEP logger socket appender interface
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

#ifndef APPENDER_TYPE_SOCKET_H_
#define APPENDER_TYPE_SOCKET_H_

#include <log4c/appender.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief MEP socket appender type definition.
 *
 * This should be used as a parameter to the log4c_appender_set_type()
 * routine to set the type of the appender.
 *
 **/
const log4c_appender_type_t log4c_appender_type_MEP_socket;

typedef struct __MEP_socket_udata MEP_socket_udata_t; /* opaque */

#define SOCKET_DEFAULT_PORT       "5000" /* IPPORT_USERRESERVED */
#define SOCKET_DEFAULT_IP_ADDRESS "127.0.0.1"

/**
 * @brief Get a new socket appender configuration object.
 * @return a new socket appender configuration object, otherwise NULL.
*/
MEP_socket_udata_t *MEP_socket_make_udata(void);

/**
 * @brief Set the port number for the socket appender configuration.
 * @param sudatap the socket appender configuration object.
 * @param port the port number to set.
 * @return zero if successful, non-zero otherwise.
 */
int MEP_socket_udata_set_port(
                MEP_socket_udata_t *sudatap,
                char* port);

/**
 * @brief Get the port number string for the socket appender configuration.
 * @param sudatap the socket appender configuration object.
 * @return the port number string.
 */
const char * MEP_socket_udata_get_port(MEP_socket_udata_t* sudatap);

/**
 * @brief Set the IP address for the socket appender configuration.
 * @param sudatap the socket appender configuration object.
 * @param ip_address the IP address to use.
 * @return zero if successful, non-zero otherwise.
 */
int MEP_socket_udata_set_ipaddress(
                MEP_socket_udata_t *sudatap,
                char* ip_adress);

/**
 * @brief Get the IP address string for the socket appender configuration.
 * @param sudatap the socket appender configuration object.
 * @return the IP address string.
 */
const char * MEP_socket_udata_get_ipaddress(MEP_socket_udata_t* sudatap);

#ifdef __cplusplus
}
#endif

#endif /* APPENDER_TYPE_SOCKET_H_ */
