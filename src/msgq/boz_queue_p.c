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
 * \file        boz_queue_p.c
 * \brief       Message Management private APIs.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <stdio.h>

#include "boz_queue_p.h"

boz_queue_internal_t boz_queue_internal_zero = BOZ_QUEUE_INTERNAL_ZERO;
boz_queue_glob_t boz_queue_g = BOZ_QUEUE_GLOB_ZERO;

//*****************************************************************************
//*****************************************************************************
static int boz_queue_free_iter(char* e, void* p) {
    boz_queue_internal_t* elem=(boz_queue_internal_t*)e;
    fprintf(stderr, "%s: %p, id(%u)\n", __PRETTY_FUNCTION__, elem, elem->id);
    (void)p;
    stralloc_free(&elem->data);
    return 1;
}

//*****************************************************************************
//*****************************************************************************
__attribute__((constructor))
static void boz_queue_init() {
    fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
    gensetdyn_init(&boz_queue_g.storage, sizeof(boz_queue_internal_t), 16, 0, 1);
}

//*****************************************************************************
//*****************************************************************************
__attribute__((destructor))
static void boz_queue_fini() {
    fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
    unsigned int n = gensetdyn_n(&boz_queue_g.storage);
    unsigned int m = gensetdyn_iter(&boz_queue_g.storage, boz_queue_free_iter, NULL);
    if(m != n) {
        fprintf(stderr, "%s: bad elems free count (expected:%d/done:%d), remains(%lu)\n", __PRETTY_FUNCTION__, n, m, gensetdyn_n(&boz_queue_g.storage));
    }
    gensetdyn_free(&boz_queue_g.storage);
}

