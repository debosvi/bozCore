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
 * \file        boz_msg_release.c
 * \brief       Message creation implementation.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>

#include "boz_msg_p.h"

boz_msg_t boz_msg_new(const unsigned int size, const boz_msg_type_t type) {
    unsigned int id=-1;
    boz_msg_internal_t *p=NULL;

    switch(type) {
    case BOZ_MSG_TYPE_RAW: 
    case BOZ_MSG_TYPE_BASIC: 
        break;
    default: 
        return (errno=EINVAL,-1);
    }

    if(!gensetdyn_new(&boz_msg_g.storage, &id))
        return (errno=ENOSPC,-1);

    p = (boz_msg_internal_t*)gensetdyn_p(&boz_msg_g.storage, id);
    memset(p, 0xff, sizeof(boz_msg_internal_t));
    p->id = id;
    p->type = type;
    p->size = size;
    p->data = stralloc_zero;

    if(size) {
        stralloc_ready(&p->data, size);
    }

    return id;
}

