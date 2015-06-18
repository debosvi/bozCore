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
 * \file        bozmessage_put.c
 * \brief       boz message put message to sender.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>
#include <skalibs/stralloc.h>
#include <skalibs/siovec.h>
#include <skalibs/uint16.h>
#include <skalibs/md5.h>
#include <bozCore/bozmessage.h>
#include "bozmessage_internal.h"

static uint16 g_put_hdr = PUT_HDR_MAGIC;

static int reserve_and_copy (bozmessage_sender_t *b, unsigned int len) {
    unsigned int cur = b->data.len;
    if (len > BOZMESSAGE_MAXSIZE)
        return (errno = EPROTO, 0) ;
    if (!genalloc_readyplus(unsigned int, &b->offsets, 1) ||
        !stralloc_readyplus(&b->data, len))
        return 0 ;
    byte_copy(b->offsets.s + b->offsets.len, sizeof(unsigned int), (char const *)&cur) ;
    b->offsets.len += sizeof(unsigned int) ;
    return 1 ;
}

static int compute_hash(bozmessage_sender_t *b, char const *s, const unsigned int len) {
    MD5Schedule hash = MD5_INIT();
    md5_update (&hash, s, len);
    md5_final(&hash, b->data.s + b->data.len);
    b->data.len += PUT_HASH_SIZE;
    return 1;
}

int bozmessage_put_and_close (bozmessage_sender_t *b, bozmessage_t const *m) {
    unsigned int tlen = m->len + PUT_HDR_TOTAL_SIZE;
    if (!reserve_and_copy(b, tlen+PUT_HASH_SIZE))
        return 0 ;
    byte_copy(b->data.s + b->data.len, PUT_HDR_MAGIC_SIZE, &g_put_hdr) ;
    uint16_pack_big(b->data.s + b->data.len + PUT_HDR_MAGIC_SIZE, m->len) ;
    byte_copy(b->data.s + b->data.len + PUT_HDR_TOTAL_SIZE, m->len, m->s) ;
    b->data.len += tlen;
    compute_hash(b, m->s, m->len);
    return 1 ;
}

int bozmessage_putv_and_close (bozmessage_sender_t *b, bozmessage_v_t const *m) {
    unsigned int len = 0 ;
    register unsigned int i = 0 ;
    for (; i < m->vlen ; i++)
        len += PUT_HDR_TOTAL_SIZE + m->v[i].len + PUT_HASH_SIZE;
    if (!reserve_and_copy(b, len))
        return 0 ;
    for (i = 0 ; i < m->vlen ; i++) {
        byte_copy(b->data.s + b->data.len, PUT_HDR_MAGIC_SIZE, &g_put_hdr) ;
        uint16_pack_big(b->data.s + b->data.len + PUT_HDR_MAGIC_SIZE, m->v[i].len) ;
        byte_copy(b->data.s + b->data.len + PUT_HDR_TOTAL_SIZE, m->v[i].len, m->v[i].s) ;
        b->data.len += m->v[i].len + PUT_HDR_TOTAL_SIZE ;
        compute_hash(b, m->v[i].s, m->v[i].len);
    }
    return 1 ;
}
