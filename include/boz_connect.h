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
 * \file        boz_connect.h
 * \brief       Message Management public APIs.
 * \version     @boz_connect_VERSION@
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 * \brief Implements a message communication between 2 endpoints
 *
 */
#ifndef _BOZ_CONNECT_H_
#define _BOZ_CONNECT_H_

/**
 * \ingroup BOZCORE
 * \defgroup BOZCONNECT Message Management public APIs.
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
    BOZ_CONNECT_TYPE_BASIC=0,	    /*!< Input/output connection. */
    BOZ_CONNECT_TYPE_READ_ONLY,	    /*!< Input only connection. */
    BOZ_CONNECT_TYPE_WRITE_ONLY,	/*!< Output connection. */
    BOZ_CONNECT_TYPE_END_VALUE
} boz_connect_type_t;

/**
 * @brief Message identifier.
 */
typedef int boz_connect_t;

/**
 * @brief Invalid message identifier value.
 */
#define BOZ_CONNECT_INVALID (-1)

/**
 * @brief Message queue parameters.
 *
 * If \ref p.size if positive, the message depth is limited, then it can't exceeds this. 
 * If \ref p.size is 0, the successive fill can grow up internal data without limitations.
 *
 */
typedef struct{
    boz_connect_type_t  type;       /*!< Type of the messages managed by the queue. */
    int                 fd;         /*!< File descriptor onto read/write. */
    unsigned int        rsize;      /*!< Size of messages to be read. */
} boz_connect_params_t;
#define BOZ_CONNECT_PARAMS_ZERO {   \
    .type=BOZ_CONNECT_TYPE_BASIC,   \
    .fd = -1,                       \
    .rsize = 0,                     \
}

extern boz_connect_params_t boz_connect_params_zero;

/**
 * @brief Create a new typed message.
 * @param[in]   p initialise parameters.
 * @return      message identifier (to be used in all others APIs afterwards).
 * @retval      >=0 in case of success.
 * @retval      BOZ_CONNECT_INVALID in case of failure, errno set accordingly.
 * 
 * errno can be :
 * - EINVAL if parameters are bad values.
 * - ENOSPC in case of no more memory available
 * - EIO if permanent failure.
 */ 
boz_connect_t boz_connect_new(const boz_connect_params_t* const p);

/**
 * @brief Release a message from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @return      operation status
 * @retval      0 on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - EIO if permanent failure.
 */ 
int boz_connect_release(const boz_connect_t id);

/**
 * @brief Get type of message from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @return      message type
 * @retval      valid type on success.
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 */ 
boz_connect_type_t boz_connect_type(const boz_connect_t id);

/**
 * @brief Get message file descriptor.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @return      message file descriptor
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 */ 
int boz_connect_fd(const boz_connect_t id);

/**
 * @brief Get message poll events mask to get from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @return      message poll events
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 */ 
int boz_connect_events(const boz_connect_t id);

/**
 * @brief Flush output data from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @return      0 on success
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSYS if \p id is open as BOZ_CONNECT_TYPE_READ_ONLY.
 */ 
int boz_connect_flush(const boz_connect_t id);

/**
 * @brief Put data on output from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @param[in]   x start data buffer.
 * @param[in]   len data length.
 * @return      0 on success
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSYS if \p id is open as BOZ_CONNECT_TYPE_READ_ONLY.
 * - EFAULT if \p x or \p len is null.
 */ 
int boz_connect_put(const boz_connect_t id, char const *x, unsigned int len);

/**
 * @brief Optimse internal usage from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @return      0 on success
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 */ 
int boz_connect_yield(const boz_connect_t id);

/**
 * @brief Get data on input from identifier.
 * @param[in]   id message identifier given with \ref boz_connect_new or \ref boz_connect_dup.
 * @param[in]   x start data buffer.
 * @param[in]   max data length.
 * @return      >0 data retrieved
 * @retval      -1 on failure, errno set accordingly.
 *
 * errno can be :
 * - ENOMSG if \p id is not a previously assigned identifier.
 * - ENOSYS if \p id is open as BOZ_CONNECT_TYPE_WRITE_ONLY.
 * - EFAULT if \p x or \p len is null.
 */ 
int boz_connect_get(const boz_connect_t id, char* x, unsigned int max);

/**
 *\}
 * End of group
 */

#ifdef __cplusplus
}
#endif

#endif   // _BOZ_CONNECT_H_ 
