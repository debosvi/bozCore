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
 * \file        boz_msg.h
 * \brief       Message Management.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 * \brief Implements a message communication between 2 endpoints
 *
 */
#ifndef _BOZ_MSG_H_
#define _BOZ_MSG_H_

/**
 * \ingroup BOZCORE
 * \defgroup BOZMSG Message Management API.
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Defines the kind of message.
 */
typedef enum {
    BOZ_MSG_TYPE_RAW=0,    /*!< No header, no footer. */
    BOZ_MSG_TYPE_BASIC,     /*!< Basic header and footer, add size in header and CRC32 in footer. */
    BOZ_MSG_TYPE_END_VALUE
} boz_msg_type_t;

/**
 * @brief Message identifier.
 */
typedef int boz_msg_t;

/**
 * @brief Invalid message identifier value.
 */
#define BOZ_MSG_INVALID (-1)

/**
 * @brief Create a new typed message.
 * @param[in]   size pre-allocated message size.
 * @param[in]   type message type.
 * @return      message identifier (to be used in all others APIs afterwards).
 * @retval      >=0 in case of success.
 * @retval      BOZ_MSG_INVALID in case of failure, errno set accordingly.
 * 
 * If \ref size if positive, the message depth is limited, then it can't exceeds this. 
 * If \ref size is 0, the successive fill can grow up internal data without limitations.
 *
 * errno can be :
 * - EINVAL if \p size or \p type are bad values.
 * - ENOSPC in case of no more memory available
 * - EIO if permanent failure.
 */ 
boz_msg_t boz_msg_new(const unsigned int size, const boz_msg_type_t type);

/**
 * @brief Release a message.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @return      operation status
 * @retval      0 on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 */ 
int boz_msg_release(const boz_msg_t id);

/**
 * @brief Get type of message.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @return      message type
 * @retval      valid type on success.
 * @retval      BOZ_MSG_TYPE_END_VALUE on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 */ 
boz_msg_type_t boz_msg_type(const boz_msg_t id);

/**
 * @brief Duplicate a message.
 * @param[in]   id message identifier given with \ref boz_msg_new (or \ref boz_msg_dup).
 * @return      message identifier (to be used in all others APIs afterwards).
 * @retval      >=0 in case of success.
 * @retval      BOZ_MSG_INVALID in case of failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSPC in case of no more memory available
 * - EIO if permanent failure.
 */ 
boz_msg_t boz_msg_dup(const boz_msg_t id);

/**
 * @brief Get message payload.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @param[out]  payload payload reference.
 * @return      operation status
 * @retval      >=0 in case of success.
 * @retval      -1 in case of failure, \p buf set to NULL, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 */ 
int boz_msg_payload(const boz_msg_t id, char** payload);

/**
 * @brief Append data to message.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @param[in]   data data reference.
 * @param[in]   lg data length.
 * @return      operation status
 * @retval      >=0 in case of success.
 * @retval      -1 in case of failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSPC if \p lg cannot be reached due to limited message size.
 * - EIO if permanent failure.
 */ 
int boz_msg_append(const boz_msg_t id, const char* data, const unsigned int lg);

/**
 * @brief Load data to message from first payload byte.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @param[in]   data data reference.
 * @param[in]   lg data length.
 * @return      operation status
 * @retval      >=0 in case of success.
 * @retval      -1 in case of failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSPC if \p lg cannot be reached due to limited message size.
 * - EIO if permanent failure.
 */ 
int boz_msg_load(const boz_msg_t id, const char* data, const unsigned int lg);

/**
 * @brief Append formatted string to message.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @param[in]   fmt string format.
 * @return      operation status
 * @retval      >0 in case of success.
 * @retval      -1 in case of failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSPC if \p lg cannot be reached due to limited message size.
 * - EIO if permanent failure.
 */ 
int boz_msg_vsprintf(const boz_msg_t id, const char *fmt, ...);

/**
 * @brief Write message payload on file descriptor, after having performed additional operations
 * depending on message type.
 * @param[in]   id message identifier given with \ref boz_msg_new or \ref boz_msg_dup.
 * @param[in]   fd file descriptor.
 * @return      operation status
 * @retval      >0 in case of success.
 * @retval      -1 in case of failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EBADF if bad file descriptor given
 * - EIO if permanent failure.
 */ 
int boz_msg_write(const boz_msg_t id, int fd);

/**
 * @brief Read message from file descriptor, before having performed additional checks 
 * depending on message type.
 * @param[in]   type message type.
 * @param[in]   fd file descriptor.
 * @return      message identifier (to be used in all others APIs afterwards), see \ref boz_msg_new.
 * @retval      >=0 in case of success.
 * @retval      BOZ_MSG_INVALID in case of failure, errno set accordingly.
 *
 * errno can be :
 * - EINVAL if \p size or \p type are bad values.
 * - ENOSPC in case of no more memory available
 * - EIO if permanent failure.
 */ 
boz_msg_t boz_msg_read(const boz_msg_type_t type, int fd);

/**
  *\}
  * End of group
  */

#ifdef __cplusplus
}
#endif

#endif   // _BOZ_MSG_H_ 
