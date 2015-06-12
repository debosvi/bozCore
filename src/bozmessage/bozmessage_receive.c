/* ISC license. */

#include <errno.h>
#include <skalibs/uint16.h>
#include <skalibs/buffer.h>
#include <skalibs/md5.h>
#include <bozCore/bozmessage.h>
#include "bozmessage_internal.h"

static int compute_and_check_hash(char const *s, const unsigned int len) {
    MD5Schedule hash = MD5_INIT();
    char c[PUT_HASH_SIZE];
    md5_update (&hash, s, len);
    md5_final(&hash, c);
    return byte_diff(s+len, len, c);
}

int bozmessage_receive (bozmessage_receiver_t *b, bozmessage_t *m) {
    register int r ;
    if (b->maindata.len == b->mainlen) {
        char pack[PUT_HDR_TOTAL_SIZE] ;
        uint16 *magic = (uint16*)&pack[0];
        if (buffer_len(&b->mainb) < PUT_HDR_TOTAL_SIZE) {
            r = sanitize_read(buffer_fill(&b->mainb)) ;
            if (r <= 0) return r ;
            if (buffer_len(&b->mainb) < PUT_HDR_TOTAL_SIZE)
                return (errno = EWOULDBLOCK, 0) ;
        }
        buffer_get(&b->mainb, pack, PUT_HDR_TOTAL_SIZE) ;
        uint16_unpack_big(pack+2, &b->mainlen) ;
        if ((b->mainlen > BOZMESSAGE_MAXSIZE) || ((*magic) != PUT_HDR_MAGIC))
            return (errno = EPROTO, -1) ;
        if (!stralloc_ready(&b->maindata, b->mainlen+PUT_HASH_SIZE))
            return -1 ;
        b->maindata.len = 0 ;
    }

    b->mainlen += PUT_HASH_SIZE;
    for (;;) {
        register unsigned int n = buffer_len(&b->mainb) ;
        if (n > b->mainlen - b->maindata.len) n = b->mainlen - b->maindata.len ;
        b->maindata.len += buffer_get(&b->mainb, b->maindata.s + b->maindata.len, n) ;
        if (b->maindata.len == b->mainlen ) break ;
        r = sanitize_read(buffer_fill(&b->mainb)) ;
        if (r <= 0) return r ;
    }

    if((r=compute_and_check_hash(b->maindata.s, b->maindata.len-PUT_HASH_SIZE))%256)
        return (errno = EPROTO, -1) ;

    m->s = b->maindata.s ;
    m->len = b->maindata.len-PUT_HASH_SIZE ;
    return 1 ;
}
