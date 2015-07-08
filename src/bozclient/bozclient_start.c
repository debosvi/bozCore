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
 * \file        bozclient_start.c
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

int bozclient_start (
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
        tain_t const *deadline,
        tain_t *stamp) {
    bozclient_cbdata_t blah ;
    bozmessage_t m ;
    register int r ;
    if (!bozclient_start_async(a, bufss, bufsn, bufas, bufan, q, qlen, fd, before, beforelen, after, afterlen, &blah)) 
        return 0 ;
    if (!bozclient_timed_flush(a, deadline, stamp))
    {
        register int e = errno ;
        bozclient_end(a) ;
        errno = e ;
        return 0 ;
    }
    r = bozmessage_timed_receive(&a->syncin, &m, deadline, stamp) ;
    if (r < 1)
    {
        int e = errno ;
        if (!r) e = EPIPE ;
        bozclient_end(a) ;
        errno = e ;
        return 0 ;
    }
    return kolbak_call((unixmessage_t*)&m, &a->kq) ;
}
