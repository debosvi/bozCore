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
 * \file        boz_queue_new.c
 * \brief       Message creation implementation.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>

#include "boz_queue_p.h"

boz_msg_queue_t boz_msg_queue_new(const boz_msg_params_t* const p) {
    unsigned int id=-1;
    boz_queue_internal_t *pi=NULL;
    boz_msg_params_t *pp=(boz_msg_params_t*)p;
    
    if(!pp)
        pp = &boz_msg_params_zero;
    
    switch(p->type) {
    case BOZ_MSG_TYPE_RAW: 
    case BOZ_MSG_TYPE_BASIC: 
        break;
    default: 
        return (errno=EINVAL,-1);
    }

    if(!gensetdyn_new(&boz_queue_g.storage, &id))
        return (errno=ENOSPC,-1);

    pi = (boz_queue_internal_t*)gensetdyn_p(&boz_queue_g.storage, id);
    (*pi) = boz_queue_internal_zero;
    pi->id = id;
    pi->params = *pp;
//    pi->data = stralloc_zero;

    if(pi->params.size) {
        stralloc_ready(&pi->data, pi->params.size);
    }

    return (errno=0,id);
}

