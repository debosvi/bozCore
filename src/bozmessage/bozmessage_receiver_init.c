/* ISC license. */

#include <errno.h>
#include <skalibs/cbuffer.h>
#include <skalibs/stralloc.h>
#include <bozCore/bozmessage.h>

int bozmessage_receiver_init (bozmessage_receiver_t *b, int fd, char *mainbuf, unsigned int mainlen) {
  if (!buffer_init(&b->mainb, buffer_read, fd, mainbuf, mainlen)) 
      return 0 ;
  b->mainlen = 0 ;
  b->maindata = stralloc_zero ;
  return 1 ;
}
