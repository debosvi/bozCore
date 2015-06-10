/* ISC license. */

#include <skalibs/sysdeps.h>
#include <skalibs/nonposix.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <skalibs/uint16.h>
#include <skalibs/uint32.h>
#include <skalibs/bytestr.h>
#include <skalibs/diuint.h>
#include <skalibs/allreadwrite.h>
#include <skalibs/genalloc.h>
#include <skalibs/djbunix.h>
#include <bozCore/bozmessage.h>

int bozmessage_sender_flush (bozmessage_sender_t *b) {
    unsigned int last = b->data.len ;
    unsigned int *offsets = genalloc_s(unsigned int, &b->offsets) ;
    unsigned int n = genalloc_len(unsigned int, &b->offsets) ;
    register int r = 0 ;

    if (b->shorty) { /* we had a short write, gotta send the remainder first */
        unsigned int *next = b->head+1 < n ? offsets + b->head+1 : &last ;
        unsigned int len = (*next) - offsets[b->head] ;
        if (b->shorty <= len)
            r = fd_write(b->fd, b->data.s + offsets[b->head] + (len - b->shorty), b->shorty) ;
        if (r <= 0) 
            return 0 ;
        b->shorty -= r ;
        if (b->shorty) 
            return (errno = EWOULDBLOCK, 0) ;
    }

    for (; b->head < n ; b->head++) {
        unsigned int *next = b->head+1 < n ? offsets + b->head+1 : &last ;
        unsigned int len = (*next) - offsets[b->head] ;
        char *head=b->data.s + offsets[b->head];
        do r = fd_write(b->fd, head, len) ;
        while (r < 0 && errno == EINTR) ;
        if (r <= 0) return 0 ;
        if ((unsigned int)r < len) {
            b->shorty = len - r ;
            return (errno = EWOULDBLOCK, 0) ;
        }
    }
    b->data.len = 0 ;
    genalloc_setlen(unsigned int, &b->offsets, 0) ;
    b->head = 0 ;
    return 1 ;
}
