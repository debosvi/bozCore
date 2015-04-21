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
 * \file        boz_connect_release.c
 * \brief       Message release implementation.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "boz_connect_p.h"

typedef struct {
    unsigned int max;
} iter_max_stuff_t;

static int iter_max (char *e, void *stuff) {
    boz_connect_internal_t *p=(boz_connect_internal_t*)e;
    iter_max_stuff_t *s = (iter_max_stuff_t*)stuff;
    if((p->id!=BOZ_CONNECT_INVALID) && (unsigned int)p->id > s->max)
        s->max = p->id;
    return 1;
}

int boz_connect_release(const boz_connect_t id) {
    boz_connect_internal_t *p=NULL;

    BOZ_CONNECT_EMPIRIC_BAD_ID_TEST

    p = GENSETDYN_P(boz_connect_internal_t, &boz_connect_g.storage, id);
    if(p->id != id)
        return (errno=ENOMSG,-1);
        
    bufalloc_free(&p->d_out);
    free(p->b_in);
    if(fcntl(p->params.fd, F_GETFD)>0)
        close(p->params.fd);
    p->b_in=0;
    (*p) = boz_connect_internal_zero;

    {
        iter_max_stuff_t stuff = { 0 };
        gensetdyn_iter(&boz_connect_g.storage, iter_max, &stuff);
        boz_connect_g.max=stuff.max;
    }

    gensetdyn_delete(&boz_connect_g.storage, id);

    return (errno=0,0);
}

