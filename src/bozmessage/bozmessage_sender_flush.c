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
 * \file        bozmessage_sender_flush.c
 * \brief       boz message sender flushing function.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <skalibs/sysdeps.h>
#include <skalibs/nonposix.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <skalibs/uint16.h>
#include <skalibs/uint32.h>
#include <skalibs/bytestr.h>
#include <skalibs/diuint.h>
#include <skalibs/allreadwrite.h>
#include <skalibs/genalloc.h>
#include <skalibs/djbunix.h>
#include <bozCore/bozmessage.h>

int bozmessage_sender_flush (bozmessage_sender_t *b) {
    unsigned int last = b->data.len ;
    unsigned int *offsets = genalloc_s(unsigned int, &b->offsets) ;
    unsigned int n = genalloc_len(unsigned int, &b->offsets) ;
    register int r = 0 ;

    if (b->shorty) { /* we had a short write, gotta send the remainder first */
        unsigned int *next = b->head+1 < n ? offsets + b->head+1 : &last ;
        unsigned int len = (*next) - offsets[b->head] ;
        if (b->shorty <= len)
            r = fd_write(b->fd, b->data.s + offsets[b->head] + (len - b->shorty), b->shorty) ;
        if (r <= 0) 
            return 0 ;
        b->shorty -= r ;
        if (b->shorty) 
            return (errno = EWOULDBLOCK, 0) ;
    }

    for (; b->head < n ; b->head++) {
        unsigned int *next = b->head+1 < n ? offsets + b->head+1 : &last ;
        unsigned int len = (*next) - offsets[b->head] ;
        char *head=b->data.s + offsets[b->head];
        do r = fd_write(b->fd, head, len) ;
        while (r < 0 && errno == EINTR) ;
        if (r <= 0) return 0 ;
        if ((unsigned int)r < len) {
            b->shorty = len - r ;
            return (errno = EWOULDBLOCK, 0) ;
        }
    }
    b->data.len = 0 ;
    genalloc_setlen(unsigned int, &b->offsets, 0) ;
    b->head = 0 ;
    return 1 ;
}
