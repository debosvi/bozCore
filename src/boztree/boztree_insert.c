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
 * \file        boztree_insert.c
 * \brief       boz message receiver initialisation.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include "boztree_internal.h"
#include "bozCore/boztree.h"

int boztree_insert(boztree_t *t, boztree_id_t const *e) {
    unsigned int n=0;
    void *p;
    if(!t) return (errno=EFAULT, -1);
    if(!e) return (errno=EINVAL, -1);
    if(avltree_search(&t->a, &e->i, &n)) return (errno=EEXIST, -1);

    if(!gensetdyn_new(&t->s, &n)) return (errno=ENOMEM, -1);
    p = gensetdyn_p(&t->s, n);
    memcpy(p, e, t->s.esize);

    avltree_insert(&t->a, n);
    
    return 0;
}
