/// @file rollingpolicy_type_complete.h
/// @brief "complete" log4c rolling file policy interface
/// @author MEP
/// @classification FR - PUBLIC MORPHO [MEP]
/// @copyright Copyright Morpho 2013 - all rights reserved.
/// @license GNU Lesser General Public License v2.1 - See the COPYING file for the terms of usage and distribution

#ifndef log4c_policy_type_complete_h
#define log4c_policy_type_complete_h

/**
 * @file rollingpolicy_type_complete.h
 *
 * @brief Log4c rolling file complete interface.
 * Log4c ships with (and defaults to) the classic completedow rollover policy:
 * this triggers rollover when files reach a maximum size.  The first file in
 * the list is
 * always the current file; when a rollover event occurs files are shifted up
 * by one position in the list--if the number of files in the list has already
 * reached the max then the oldest file is rotated out of the window.
 *
 * If the max file size is set to zero, this means 'no-limit'.
 *
 * The default parameters for the complete policy are 5 files of maximum
 * size of 20kilobytes each.  These parameters may be changed using the
 * appropriate setter functions.
 */
 
#include <log4c/rollingpolicy.h>

#ifdef __cplusplus
extern "C" {
#endif

const log4c_rollingpolicy_type_t log4c_rollingpolicy_type_complete;

/**
 * log4c complete rolling policy type 
*/
typedef struct __complete_udata rollingpolicy_complete_udata_t;

/*#define ROLLINGPOLICY_SIZE_DEFAULT_MAX_FILE_SIZE 1024*2
#define ROLLINGPOLICY_SIZE_DEFAULT_MAX_NUM_FILES 5
*/

/**
 * Get a new complete rolling policy
 * @return a new complete rolling policy, otherwise NULL.
 */
rollingpolicy_complete_udata_t *complete_make_udata(void);

/**
 * Set the maximum file size in this rolling policy configuration.
 * @param swup the complete configuration object.
 * @param max_size the approximate maximum size any logging file will
 * attain.
 * If you set zero then it means 'no-limit' and so only one file
 * of unlimited size will be used for logging.
 * @return zero if successful, non-zero otherwise.
 */
int complete_udata_set_file_maxsize(
                              rollingpolicy_complete_udata_t * swup,
			      long max_size);
                                                            
/**
 * Set the maximum number of filesin this rolling policy configuration.
 * @param swup the complete configuration object.
 * @param max_num the maximum number of files in the list.
 * @return zero if successful, non-zero otherwise.
 */                                                         
int complete_udata_set_max_num_files(
                              rollingpolicy_complete_udata_t * swup,
	                      long max_num);

/**
 * Set the rolling file appender in this rolling policy configuration.
 * @param swup the complete configuration object.
 * @param app the rolling file appender to set.
 * @return zero if successful, non-zero otherwise.
*/                                                            
int complete_udata_set_appender(
                              rollingpolicy_complete_udata_t * swup,
			      log4c_appender_t* app);

/**
 * Delete the log files in this rolling policy configuration.
 * @param swup the complete configuration object.
 * @return zero if successful, non-zero otherwise.
*/     
int delete_log_files(rollingpolicy_complete_udata_t * swup);

#ifdef __cplusplus
}
#endif

#endif
