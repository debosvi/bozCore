// The MIT License (MIT)

// Copyright (c) 2013 Vincent de RIBOU <belzo2005-dolphin@yahoo.fr>

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*!
 * \file        boztree.h
 * \brief       boz AVL tree public APIs.
 * \version     @boztree_VERSION@
 * \date        2013/01/14
 * \author      Vincent de RIBOU.
 * \copyright   Aquaplouf Land.
 *
 * \brief Implements a message communication between 2 endpoints
 *
 */

#ifndef BOZTREE_H
#define BOZTREE_H

#include <skalibs/uint64.h>
#include <skalibs/genset.h>
#include <skalibs/avltree.h>

 /* Identifier */

typedef struct boztree_id_s boztree_id_t, *boztree_id_t_ref ;
struct boztree_id_s {
    uint64      i;
} ;
#define BOZTREE_ID_ZERO { .i = 0 }
extern boztree_id_t const boztree_id_zero ;

 /* Tree */

typedef struct boztree_s boztree_t, *boztree_t_ref ;
struct boztree_s {
    gensetdyn   s;
    avltree     a;
} ;
#define BOZTREE_ZERO { .s = GENSETDYN_ZERO, .a = AVLTREE_ZERO }
extern boztree_t const boztree_zero ;

int boztree_init(boztree_t *t, const unsigned int se);
#define BOZTREE_INIT(tree, type) boztree_init(tree, sizeof(type))
int boztree_free(boztree_t *t);

int boztree_insert(boztree_t *t, boztree_id_t const *e);
int boztree_check(boztree_t *t, const uint64 id);
void* boztree_data(boztree_t *t, const uint64 id);
int boztree_delete(boztree_t *t, const uint64 id);

#endif
