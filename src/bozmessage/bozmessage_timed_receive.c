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
 * \file        bozmessage_timed_receive.c
 * \brief       boz message receiver timed receive function.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <skalibs/functypes.h>
#include <skalibs/tai.h>
#include <skalibs/unix-timed.h>
#include <bozCore/bozmessage.h>

typedef struct bozmessage_get_s bozmessage_get_t, *bozmessage_get_t_ref ;
struct bozmessage_get_s {
    bozmessage_receiver_t *b ;
    bozmessage_t *m ;
};
 
static int getfd (bozmessage_get_t *g) {
    return bozmessage_receiver_fd(g->b) ;
}

static int get (bozmessage_get_t *g) {
    return bozmessage_receive(g->b, g->m) ;
}

int bozmessage_timed_receive (bozmessage_receiver_t *b, bozmessage_t *m, tain_t const *deadline, tain_t *stamp) {
    bozmessage_get_t g = { .b = b, .m = m } ;
    return timed_get(&g, 
        (initfunc_t *)&getfd, 
        (initfunc_t *)&get, 
        deadline, stamp) ;
}
