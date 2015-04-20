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
 * \file        boz_connect_new.c
 * \brief       Message creation implementation.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "boz_connect_p.h"

boz_connect_t boz_connect_new(const boz_connect_params_t* const p) {
    unsigned int id=-1;
    boz_connect_internal_t *pi=NULL;
    boz_connect_params_t *pp=(boz_connect_params_t*)p;
    void *buf=0;
    
    if(!pp)
        pp = &boz_connect_params_zero;
    
    switch(p->type) {
    case BOZ_CONNECT_TYPE_BASIC: 
        break;
    default: 
        return (errno=EINVAL,-1);
    }

    if(fcntl(pp->fd, F_GETFD)<0)
        return (errno=EBADF,-1);

    if(!gensetdyn_new(&boz_connect_g.storage, &id))
        return (errno=ENOSPC,-1);

    pi = (boz_connect_internal_t*)gensetdyn_p(&boz_connect_g.storage, id);
    (*pi) = boz_connect_internal_zero;
    pi->id = id;
    pi->params = *pp;
    if(!pi->params.fw)
        pi->params.fw = boz_connect_write_io;
    if(!pi->params.fr)
        pi->params.fr = boz_connect_read_io;
//    pi->data = stralloc_zero;

    buf=malloc(pi->params.rsize);
    if(!buf)
        return (errno=ENOSPC,-1);

    pi->b_in = buf;
    buffer_init(&pi->d_in, boz_connect_read_ioska, pi->params.fd, pi->b_in, pi->params.rsize);
    bufalloc_init(&pi->d_out, pi->params.fw, pi->params.fd);

    return (errno=0,id);
}

