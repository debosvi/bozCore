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
 * \file        bozconnection.h
 * \brief       boz connection public APIs.
 * \version     @bozconnection_VERSION@
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 * \brief Implements a message communication between 2 endpoints
 *
 */

#ifndef BOZCONNECTION_H
#define BOZCONNECTION_H

#include <bozCore/bozmessage.h>

typedef struct bozconnection_s bozconnection_t, *bozconnection_t_ref ;
struct bozconnection_s
{
  bozmessage_sender_t out ;
  bozmessage_receiver_t in ;
  char mainbuf[BOZMESSAGE_BUFSIZE] ;
} ;
#define BOZCONNECTION_ZERO { .out = BOZMESSAGE_SENDER_ZERO, .in = BOZMESSAGE_RECEIVER_ZERO }
extern bozconnection_t const bozconnection_zero ;

extern void bozconnection_init (bozconnection_t *, int, int) ;
extern void bozconnection_free (bozconnection_t *) ;

#define bozconnection_flush(io) bozmessage_sender_flush(&(io)->out)
#define bozconnection_receive(io, m) bozmessage_receive(&(io)->in, m)

#endif
