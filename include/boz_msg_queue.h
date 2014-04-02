// The MIT License (MIT)

// Copyright (c) 2013 Vincent de RIBOU <belzo2005-dolphin@yahoo.fr>

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*!
 * \file        boz_msg_queue.h
 * \brief       Message Queue Management.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 * \brief Implements a message queue communication between 2 endpoints
 *
 */
#ifndef _BOZ_MSG_QUEUE_H_
#define _BOZ_MSG_QUEUE_H_

#include "boz_msg.h"

/**
 * \ingroup BOZCORE
 * \defgroup BOZMSG_QUEUE Message Queue Management API.
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Message Queue identifier.
 */
typedef int boz_msg_queue_t;

/**
 * @brief Invalid message queue identifier value.
 */
#define BOZ_MSG_QUEUE_INVALID (-1)

/**
 * @brief Create a new message queue.
 * @param[in]   p initialise parameters.
 * @return      queue identifier (to be used in all others APIs afterwards).
 * @retval      >=0 in case of success.
 * @retval      BOZ_MSG_QUEUE_INVALID in case of failure, errno set accordingly.
 *
 * errno can be :
 * - EINVAL if parameters are bad values.
 * - ENOSPC in case of no more memory available
 * - EIO if permanent failure.
 */ 
boz_msg_queue_t boz_msg_queue_new(const boz_msg_params_t* const p);

/**
 * @brief Release a message queue.
 * @param[in]   id message queue identifier given with \ref boz_msg_queue_new.
 * @return      operation status
 * @retval      0 on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 */ 
int boz_msg_queue_release(const boz_msg_queue_t id);

/**
 * @brief Add a message to queue.
 * @param[in]   idq message queue identifier given with \ref boz_msg_queue_new.
 * @param[in]   idm message identifier given with \ref boz_msg_new.
 * @return      operation status
 * @retval      0 on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - EINVAL if \p idq is not a previously assigned identifier.
 * - ENOMSG if \p idm is not a previously assigned identifier.
 * - EIO if permanent failure.
 * 
 * @note the given message is inserted at the end of the queue. Do not release it after a call to this method.
 */
int boz_msg_queue_push_msg(const boz_msg_queue_t idq, const boz_msg_t idm);


/**
 * @brief Get (but not remove) a message from queue.
 * @param[in]   id message queue identifier given with \ref boz_msg_queue_new.
 * @return      operation status
 * @retval      0 on success.
 * @retval      BOZ_MSG_INVALID on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 * 
 * @note the returned message must then be released via the method #boz_msg_rel.
 */
boz_msg_t boz_msg_queue_first_msg(const boz_msg_queue_t id);

/**
 * @brief Get and remove a message from queue.
 * @param[in]   id message queue identifier given with \ref boz_msg_queue_new.
 * @return      operation status
 * @retval      0 on success.
 * @retval      BOZ_MSG_INVALID on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 */
boz_msg_t boz_msg_queue_pop_msg(const boz_msg_queue_t id);

/**
 * @brief Check if queue is empty.
 * @param[in]   id message queue identifier given with \ref boz_msg_queue_new.
 * @return      operation status
 * @retval      0 on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 * 
 * @note the returned message must then be released via the method #boz_msg_rel.
 */
int boz_msg_queue_is_empty(const boz_msg_queue_t id);

/**
 * @brief Remove all the message from the queue.
 * @param[in]   id message queue identifier given with \ref boz_msg_queue_new.
 * @return      operation status
 * @retval      0 on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 */
int boz_msg_queue_shrink(const boz_msg_queue_t id);


#if 0
/**
** @brief Interates on the messages of the given message queue.
** @param[in] msg_queue    the message queue.
** @param[in] msg_op       the operation called for each messgae.
** @param[in] msg_uc       the user context returned back in the operation.
** @retval    -1           in case of failure.
** @retval    0            in case of success.
**/
int boz_msg_queue_iterate(boz_msg_queue_t msg_queue, boz_msg_op_t msg_op, void *msg_uc);

/**
** @brief Flushes the queue through the given fd.
** @param[in] msg_queue         The message queue to flush.
** @param[in] fd                The file descriptor in which the message queue must be flushed.
** @return 0                    in case of success.
** @return 1                    if some data has been flushed but there's still unflushed data or messages.
** @return -1                   in case of failure.
**
** @note After writing the msg of the msg queue in the file descriptor, 
** the method boz_msg_queue_flush releases the messages. If its return 
** value is 0, 
**/
int boz_msg_queue_flush(boz_msg_queue_t msg_queue, int fd);


/**
** @brief Reads data from the given fd..
**/
int boz_msg_queue_read(boz_msg_queue_t msg_queue, int fd);

/**
** @brief Defines the message queue iterate operation.
** @param[in] msg     a message.
** @param[in] uc      User context given to the method #boz_msg_queue_iterate.
** @retval    -1      Indicates to the method #boz_msg_queue_iterate that the iteration must stop.
** @retval    0       Indicates to the method #boz_msg_queue_iterate that the iteration shoud continue.
**
**/
typedef int (*boz_msg_op_t)(boz_msg_t msg, void * uc);
#endif

/**
 *\}
 * End of group
 */

#ifdef __cplusplus
}
#endif

#endif   // _BOZ_MSG_QUEUE_H_

