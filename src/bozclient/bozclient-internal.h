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
 * \file        bozclient-internal.h
 * \brief       boz message receiver receive function.
 * \version     0.1
 * \date        2015/06/15
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#ifndef BOZCLIENT_INTERNAL_H
#define BOZCLIENT_INTERNAL_H

/**
 * \ingroup BOZCORE
 * \defgroup BOZCLIENT Message Management public APIs.
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <skalibs/kolbak.h>

#include <bozCore/bozmessage.h>
#include <bozCore/bozclient.h>

extern int bozclient_init (bozclient_t *, int, char *, unsigned int, char *, unsigned int, kolbak_closure_t *, unsigned int, char const *, unsigned int) ;
extern int bozclient_start_cb (bozmessage_t const *, bozclient_cbdata_t *) ;

/**
 *\}
 * End of group
 */

#ifdef __cplusplus
}
#endif

#endif // BOZCLIENT_INTERNAL_H

