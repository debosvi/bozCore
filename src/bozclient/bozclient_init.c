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
 * \file        bozclient_init.c
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>

#include <bozCore/bozmessage.h>
#include <bozCore/bozclient.h>

#include "bozclient-internal.h"

int bozclient_init (
        bozclient_t *a,
        int fd,
        char *bufss,
        unsigned int bufsn,
        char *bufas,
        unsigned int bufan,
        kolbak_closure_t *q,
        unsigned int qlen,
        char const *before,
        unsigned int beforelen) {
    bozmessage_t msg = { .s = (char *)before, .len = beforelen } ;
    if( !bozmessage_receiver_init(&a->syncin, fd, bufss, bufsn) ||
        !bozmessage_receiver_init(&a->asyncin, -1, bufas, bufan) ||
        !kolbak_queue_init(&a->kq, q, qlen)) return 0 ;
    bozmessage_sender_init(&a->syncout, fd) ;
    bozmessage_sender_init(&a->asyncout, -1) ;
    if (!bozmessage_put(&a->syncout, &msg)) return 0 ;
    return 1 ;
}