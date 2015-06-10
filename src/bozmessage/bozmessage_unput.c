 /* ISC license. */

#include <skalibs/genalloc.h>
#include <skalibs/diuint.h>
#include <skalibs/djbunix.h>
#include <bozCore/bozmessage.h>

int bozmessage_unput (bozmessage_sender_t *b) {
  unsigned int *start ;
  unsigned int n = genalloc_len(unsigned int, &b->offsets) ;
  if (!n) return 0 ;
  start = genalloc_s(unsigned int, &b->offsets) + n - 1 ;
  b->data.len = (*start) ;
  genalloc_setlen(unsigned int, &b->offsets, n-1) ;
  return 1 ;
}
