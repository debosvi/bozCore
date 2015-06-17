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
 * \file        bozmessage_internal.h
 * \brief       boz message internal stuffs.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#ifndef _BOZMESSAGE_INTERNAL_H_
#define _BOZMESSAGE_INTERNAL_H_

#include <skalibs/uint16.h>

#define PUT_HDR_MAGIC       ((uint16)0xF7A5)
#define PUT_HDR_MAGIC_SIZE  (sizeof(uint16))
#define PUT_HDR_LEN_SIZE    (sizeof(uint16))
#define PUT_HDR_TOTAL_SIZE  (PUT_HDR_MAGIC_SIZE + PUT_HDR_LEN_SIZE)
#define PUT_HASH_SIZE       (16)

#endif /* _BOZMESSAGE_INTERNAL_H_ */
