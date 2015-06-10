/* ISC license. */

#include <errno.h>
#include <skalibs/cbuffer.h>
#include <skalibs/stralloc.h>
#include <bozCore/bozmessage.h>

int bozmessage_receiver_init (bozmessage_receiver_t *b, int fd, char *mainbuf, unsigned int mainlen) {
  if (!cbuffer_init(&b->mainb, mainbuf, mainlen)) 
      return 0 ;
  b->fd = fd ;
  b->mainlen = 0 ;
  b->maindata = stralloc_zero ;
  return 1 ;
}
