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
 * \file        bozclient_putmsg.c
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#include <errno.h>
#include <skalibs/kolbak.h>

#include <bozCore/bozmessage.h>
#include <bozCore/bozclient.h>

int bozclient_putmsg_and_close (bozclient_t *a, bozmessage_t const *m, bozmessage_handler_func_t *cb, void *result) {
    if (!kolbak_enqueue(&a->kq, (unixmessage_handler_func_t*)cb, result)) return 0 ;
    if (!bozmessage_put_and_close(&a->syncout, m)) {
        kolbak_unenqueue(&a->kq) ;
        return 0 ;
    }
    return 1 ;
}
