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
 * \file        boz_msg_dup.c
 * \brief       Message duplicate implementation.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>

#include "boz_msg_p.h"

boz_msg_t boz_msg_dup(const boz_msg_t id) {
    boz_msg_internal_t *p=NULL;
    boz_msg_internal_t *p2=NULL;
    boz_msg_t nid=BOZ_MSG_INVALID;

    p = GENSETDYN_P(boz_msg_internal_t, &boz_msg_g.storage, id);
    if(p->id != id)
        return (errno=ENOMSG,-1);

    nid = boz_msg_new(p->size, p->type);
    if(nid==BOZ_MSG_INVALID) {
       return -1; 
    }

    p2 = GENSETDYN_P(boz_msg_internal_t, &boz_msg_g.storage, nid);
    if(p->id != id)
        return (errno=EIO,-1);

    stralloc_copy(&p2->data, &p->data);
        
    return (errno=0,nid);
}

