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
 * \file        bozancillary.h
 * \brief       boz ancillary public APIs.
 * \version     @BOZANCILLARY_VERSION@
 * \date        2015/07/22
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 * \brief Implements ancillary sending and receiving through socket
 *
 */

#ifndef BOZANCILLARY_H
#define BOZANCILLARY_H

#define BOZ_ANCIL_RECV_MAXFDS (32)

/**
 * @brief Send array of fds.
 * @param[in]     sock socket support to send through.
 * @param[in]     fds array of fds.
 * @param[in]     nfds array size.
 * @return        message identifier (to be used in all others APIs afterwards).
 * @retval        0 in case of success.
 * @retval        -1 in case of failure, errno set accordingly.
 * 
 * errno can be :
 * - such from "sendmsg" system call.
 */ 
extern int boz_ancil_send_fds (const int sock, const int const *fds, const unsigned int nfds);

/**
 * @brief Receive array of fds.
 * @param[in]     sock socket support to send through.
 * @param[in/out] fds array of fds.
 * @param[in]     maxfds array max size.
 * @return        message identifier (to be used in all others APIs afterwards).
 * @retval        >=0 in case of success (count of filled fds into array).
 * @retval        -1 in case of failure, errno set accordingly.
 * 
 * errno can be :
 * - such from "recvmsg" system call.
 * 
 * If maximum fds reached, return \ref maxfds but still remains fds received but not filled into array.
 */ 
extern int boz_ancil_recv_fds (const int sock, const int const *fds, const unsigned int maxfds);

#endif // BOZANCILLARY_H
