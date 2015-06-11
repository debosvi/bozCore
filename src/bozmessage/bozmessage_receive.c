/* ISC license. */

#include <skalibs/sysdeps.h>
#include <skalibs/nonposix.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <skalibs/uint16.h>
#include <skalibs/uint32.h>
#include <skalibs/cbuffer.h>
#include <skalibs/djbunix.h>
#include <skalibs/error.h>
#include <skalibs/allreadwrite.h>
#include <skalibs/stralloc.h>
#include <skalibs/siovec.h>
#include <skalibs/md5.h>
#include <bozCore/bozmessage.h>

static int compute_and_check_hash(char const *s, const unsigned int len) {
    MD5Schedule hash = MD5_INIT();
    char c[16];
    md5_update (&hash, s, len);
    md5_final(&hash, c);
    return byte_diff(s+len, len, c);
}

int bozmessage_receive (bozmessage_receiver_t *b, bozmessage_t *m) {
    if (b->maindata.len == b->mainlen) {
        char pack[4] ;
        uint16 *magic = (uint16*)&pack[0];
        if (buffer_len(&b->mainb) < 4) {
            register int r = sanitize_read(buffer_fill(&b->mainb)) ;
            if (r <= 0) return r ;
            if (buffer_len(&b->mainb) < 4) return (errno = EWOULDBLOCK, 0) ;
        }
        buffer_get(&b->mainb, pack, 4) ;
        uint16_unpack_big(pack+2, &b->mainlen) ;
        if ((b->mainlen > BOZMESSAGE_MAXSIZE) || ((*magic) != 0xf7A5))
            return (errno = EPROTO, -1) ;
        if (!stralloc_ready(&b->maindata, b->mainlen))
            return -1 ;
        b->maindata.len = 0 ;
    }

    b->mainlen +=16;
    for (;;) {
        register int r ;
        register unsigned int n = buffer_len(&b->mainb) ;
        if (n > b->mainlen - b->maindata.len) n = b->mainlen - b->maindata.len ;
        b->maindata.len += buffer_get(&b->mainb, b->maindata.s + b->maindata.len, n) ;
        if (b->maindata.len == b->mainlen ) break ;
        r = sanitize_read(buffer_fill(&b->mainb)) ;
        if (r <= 0) return r ;
    }

    if(compute_and_check_hash(b->maindata.s, b->maindata.len-16))
        return (errno = EPROTO, -1) ;

    m->s = b->maindata.s ;
    m->len = b->maindata.len-16 ;
    return 1 ;
}
