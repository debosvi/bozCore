/* ISC license. */

#include <skalibs/sysdeps.h>
#include <skalibs/diuint.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <skalibs/djbunix.h>
#include <bozCore/bozmessage.h>

void bozmessage_sender_free (bozmessage_sender_t *b) {
    genalloc_free(unsigned int, &b->offsets) ;
    stralloc_free(&b->data) ;
    *b = bozmessage_sender_zero ;
}
