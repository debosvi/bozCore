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
 * \file        boz_msg_append.c
 * \brief       Message append implementation.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <netinet/in.h>
#include <errno.h>

#include "boz_msg_p.h"

int boz_msg_append(const boz_msg_t id, const char* data, const unsigned int lg) {
    boz_msg_internal_t *p=NULL;

    p = GENSETDYN_P(boz_msg_internal_t, &boz_msg_g.storage, id);
    if(p->id != id)
        return (errno=ENOMSG,-1);

    if(!stralloc_catb(&p->data, data, lg))
        return (errno=EIO,-1);

    return (errno=0, lg);
}

int boz_msg_append_ushort(const boz_msg_t id, const unsigned short int u) {
    unsigned short int v=htons(u);
    return boz_msg_append(id, (const char*)&v, 2);
}

int boz_msg_append_ulong(const boz_msg_t id, const unsigned long int u) {
    unsigned long int v=htonl(u);
    return boz_msg_append(id, (const char*)&v, 4);
}
    
