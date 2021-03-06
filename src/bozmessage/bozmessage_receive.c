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
 * \file        bozmessage_receive.c
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>
#include <skalibs/uint16.h>
#include <skalibs/buffer.h>
#include <skalibs/md5.h>

#include <bozCore/bozmessage.h>
#include "bozmessage_internal.h"

static int compute_and_check_hash(char const *s, const unsigned int len) {
    MD5Schedule hash = MD5_INIT();
    char c[PUT_HASH_SIZE];
    md5_update (&hash, s, len);
    md5_final(&hash, c);
    return byte_diff(s+len, PUT_HASH_SIZE, c);
}

int bozmessage_receive (bozmessage_receiver_t *b, bozmessage_t *m) {
    if (b->maindata.len == b->mainlen) {
        char pack[PUT_HDR_TOTAL_SIZE] ;
        uint16 *magic = (uint16*)&pack[0];
        if (buffer_len(&b->mainb) < PUT_HDR_TOTAL_SIZE) {
            register int r = sanitize_read(buffer_fill(&b->mainb)) ;
            if (r <= 0) return r ;
            if (buffer_len(&b->mainb) < PUT_HDR_TOTAL_SIZE)
                return (errno = EWOULDBLOCK, 0) ;
        }
        buffer_get(&b->mainb, pack, PUT_HDR_TOTAL_SIZE) ;
        uint16_unpack_big(pack+2, &b->mainlen) ;
        if ((b->mainlen > BOZMESSAGE_MAXSIZE) || ((*magic) != PUT_HDR_MAGIC))
            return (errno = EPROTO, -1) ;
        if (!stralloc_ready(&b->maindata, b->mainlen+PUT_HASH_SIZE))
            return -1 ;
        b->maindata.len = 0 ;
    }

    b->mainlen += PUT_HASH_SIZE;
    for (;;) {
        register int r ;
        register unsigned int n = buffer_len(&b->mainb) ;
        if (n > b->mainlen - b->maindata.len) n = b->mainlen - b->maindata.len ;
        b->maindata.len += buffer_get(&b->mainb, b->maindata.s + b->maindata.len, n) ;
        if (b->maindata.len == b->mainlen ) break ;
        r = sanitize_read(buffer_fill(&b->mainb)) ;
        if (r <= 0) return r ;
    }

    if(compute_and_check_hash(b->maindata.s, b->maindata.len-PUT_HASH_SIZE))
        return (errno = EPROTO, -1) ;

    m->s = b->maindata.s ;
    m->len = b->maindata.len-PUT_HASH_SIZE ;
    return 1 ;
}
