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
 * \file        boztree_dtok_f.c
 * \brief       boz message receiver initialisation.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include "boztree_internal.h"
#include "bozCore/boztree.h"

#ifdef BOZTREE_DEBUG
#include <stdio.h>
#endif

#define ID_LESS(a,b) (((a)<(b)) ? -1 : ((b)<(a)))

void* boztree_dtok_f (unsigned int d, void *x) {
#ifdef BOZTREE_DEBUG
    {
        boztree_id_t *p = (boztree_id_t*)gensetdyn_p((gensetdyn*)x, d);
        fprintf(stderr, "INFO: %s: %d/%016llx\n", __PRETTY_FUNCTION__, d, (long long int)p->i);
    }
#endif

    return (void *)&((boztree_id_t*)gensetdyn_p((gensetdyn*)x, d))->i;
}
