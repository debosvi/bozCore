/* ISC license. */

#include <skalibs/cbuffer.h>
#include <skalibs/stralloc.h>
#include <skalibs/djbunix.h>
#include <bozCore/bozmessage.h>

void bozmessage_receiver_free (bozmessage_receiver_t *b) {
  b->fd = -1 ;
  stralloc_free(&b->maindata) ;
}
