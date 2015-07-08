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
 * \file        bozclient_server_ack.c
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>
#include <skalibs/bytestr.h>

#include <bozCore/bozmessage.h>
#include <bozCore/bozclient.h>

int bozclient_server_ack (bozmessage_t const *clientmsg, bozmessage_sender_t *out, bozmessage_sender_t *asyncout, char const *before, unsigned int beforelen, char const *after, unsigned int afterlen) {
    bozmessage_t m = { .s = (char *)after, .len = afterlen } ;
    (void)asyncout;

    if( (clientmsg->len != beforelen) || 
        byte_diff(clientmsg->s, beforelen, before)) return (errno = EPROTO, 0) ;

    if (!bozmessage_put_and_close(out, &m)) return 0 ;

    return 1 ;
}

