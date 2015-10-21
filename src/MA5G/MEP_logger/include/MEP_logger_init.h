/// @file MEP_logger_init.h
/// @brief Logger constructors and desctructors
/// @author MEP
/// @classification FR - INDUSTRY CONFIDENTIAL MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

#ifndef __mep_init_h
#define __mep_init_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * constructor
 * 
 * @returns 0 for success 
 **/
int MEP_log_init_with_conf_file(const char* config_file);

/**
 * destructor
 *
 * @returns 0 for success 
 **/
int MEP_log_fini(void);

/*
 * Dumps all the current appender, layout and rollingpolicy types
 * known by log4c.
 * @param stream to write to
 */
void MEP_log_dump_all_types(FILE *fp);

/*
 * Dumps all the current instances of categories, appenders, layouts
 * and rollingpolicy objects.
 * An instances of a type consists of the base
 * type information (name plus function table) and an instance name and
 * configuration.  For example one can have an instance of the rollingfile
 * appender which logs to /var/tmp and another instance which logs to 
 * /usr/tmp.  They are both of type rollingfile, but are distinct instances of
 * it
 * @param stream to write t
 */
void MEP_log_dump_all_instances(FILE *fp);

#ifdef __cplusplus
}
#endif

#endif
