/// @file MEP_log_rc.h
/// @brief Logger resource configuration interface
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution


#ifndef __MEP_log_rc_h
#define __MEP_log_rc_h

#include <log4c/rc.h>

#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @brief MEP log rolling files delete
 * @param appender name of which the log files are to be deleted
 * @return 0 for success
 **/
int MEP_log_rc_delete(const char* appender_name);
/**
 * load log4c resource configuration file
 *
 * @param a_filename name of file to load
 **/
int	MEP_load(const char* a_filename);

/**
 * @internal
 **/
int	MEP_rc_load(log4c_rc_t* a_rc, const char* a_filename);

#ifdef __cplusplus
}
#endif

#endif
