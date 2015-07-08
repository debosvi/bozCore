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
 * \file        bozclient_server_init.c
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <skalibs/tai.h>
#include <skalibs/allreadwrite.h>

#include <bozCore/bozmessage.h>
#include <bozCore/bozclient.h>

int bozclient_server_init (bozmessage_receiver_t *in, bozmessage_sender_t *out, bozmessage_sender_t *asyncout, char const *before, unsigned int beforelen, char const *after, unsigned int afterlen, tain_t const *deadline, tain_t *stamp) {
    bozmessage_t m ;
    return (sanitize_read(bozmessage_timed_receive(in, &m, deadline, stamp)) >= 0)
        && bozclient_server_ack(&m, out, asyncout, before, beforelen, after, afterlen)
        && bozmessage_sender_timed_flush(out, deadline, stamp) ;
}

