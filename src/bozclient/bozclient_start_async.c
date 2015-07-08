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
 * \file        bozclient_start_async.c
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>
#include <skalibs/djbunix.h>

#include <bozCore/bozmessage.h>
#include <bozCore/bozclient.h>

#include "bozclient-internal.h"

int bozclient_start_async (
        bozclient_t *a,
        char *bufss,
        unsigned int bufsn,
        char *bufas,
        unsigned int bufan,
        kolbak_closure_t *q,
        unsigned int qlen,
        const int fd,
        char const *before,
        unsigned int beforelen,
        char const *after,
        unsigned int afterlen,
        bozclient_cbdata_t *blah) {
    if (!bozclient_init(a, fd, bufss, bufsn, bufas, bufan, q, qlen, before, beforelen)) {
        register int e = errno ;
        fd_close(fd) ;
        errno = e ;
        return 0 ;
    }
    if (!kolbak_enqueue(&a->kq, (unixmessage_handler_func_t_ref)&bozclient_start_cb, blah))
    {
        bozclient_end(a) ;
        return 0 ;
    }
    blah->a = a ;
    blah->after = after ;
    blah->afterlen = afterlen ;
    return 1 ;
}
