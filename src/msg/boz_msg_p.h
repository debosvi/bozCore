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
 * \file        boz_msg_p.h
 * \brief       Message Management private APIs.
 * \version     0.1
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 */

#ifndef _BOZ_MSG_PRIV_H_
#define _BOZ_MSG_PRIV_H_

/**
 * \ingroup BOZCORE_PRIV
 * \defgroup BOZMSG_PRIV Message Management private APIs.
 * \{
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <skalibs/gensetdyn.h>

#include "boz_msg.h"
  
/**
 * @brief Messsage internal type.
 */
typedef struct {
    boz_msg_t           id;         /*!< message storage id. */
    boz_msg_params_t    params;     /*!< message params. */
    stralloc            data;       /*!< message data. */
} boz_msg_internal_t;

#define BOZ_MSG_INTERNAL_ZERO { .id=BOZ_MSG_INVALID, .params=BOZ_MSG_PARAMS_ZERO, .data=STRALLOC_ZERO }
extern boz_msg_internal_t boz_msg_internal_zero;
  
/**
 * @brief Messsage global type.
 */
typedef struct {
    gensetdyn storage;      /*!< messages storage. */
} boz_msg_glob_t;

/**
 * @brief Messsage global type initializer.
 */
#define BOZ_MSG_GLOB_ZERO { .storage=GENSETDYN_ZERO }

/**
 * @brief Messsage global type instance.
 */
extern boz_msg_glob_t boz_msg_g;

/**
 *\}
 * End of group
 */

#ifdef __cplusplus
}
#endif

#endif   // _BOZ_MSG_H_ 
