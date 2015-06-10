/* ISC license. */

#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <bozCore/bozmessage.h>

void bozmessage_sender_init (bozmessage_sender_t *b, int fd) {
    b->fd = fd ;
    b->data = stralloc_zero ;
    b->offsets = genalloc_zero ;
}
