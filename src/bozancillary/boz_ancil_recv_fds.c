/*
The MIT License (MIT)

Copyright (c) 2013 Vincent de RIBOU <belzo2005-dolphin@yahoo.fr>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*!
 * \file        bozmessage_handle.c
 * \brief       boz message receiver handle function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include "boz_ancillary_p.h"

int boz_ancil_recv_fds (const int sock, const int const *fds, const unsigned int maxfds) {
  char ancilbuf[BOZ_ANCIL_RECV_MAXFDS*CMSG_SPACE(sizeof(int))] ;
  char s[BOZ_ANCIL_ID_LG] ;
  struct iovec v = { .iov_base = s, .iov_len = BOZ_ANCIL_ID_LG } ;
  struct msghdr msghdr =
  {
    .msg_name = 0,
    .msg_namelen = 0,
    .msg_iov = &v,
    .msg_iovlen = 1,
    .msg_flags = 0,
    .msg_control = ancilbuf,
    .msg_controllen = sizeof(ancilbuf)
  } ;
  struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msghdr) ;
  unsigned int rnfds;
  
  if(maxfds>BOZ_ANCIL_RECV_MAXFDS) return (errno=ENOMEM, -1);
  
  cmsg->cmsg_len = msghdr.msg_controllen ;
  cmsg->cmsg_level = SOL_SOCKET ;
  cmsg->cmsg_type = SCM_RIGHTS ;
  
  memset(CMSG_DATA(cmsg), -1, BOZ_ANCIL_RECV_MAXFDS);
  
  if (recvmsg(sock, &msghdr, 0) != 8) return -1 ;
  if (memcmp(s, boz_ancil_identifier, BOZ_ANCIL_ID_LG)) return (errno=EIO,-1) ;
  
  rnfds = (cmsg->cmsg_len-sizeof(*cmsg))/sizeof(int);
  if(rnfds>maxfds) rnfds = maxdfs;
  memcpy(fds, CMSG_DATA(cmsg), rnfds);
  return (errno=0,rnfds);

}

