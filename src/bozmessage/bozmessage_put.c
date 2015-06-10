/* ISC license. */

#include <errno.h>
#include <skalibs/bitarray.h>
#include <skalibs/bytestr.h>
#include <skalibs/stralloc.h>
#include <skalibs/siovec.h>
#include <bozCore/bozmessage.h>

static int reserve_and_copy (bozmessage_sender_t *b, unsigned int len) {
    unsigned int cur = b->data.len;
    if (len > BOZMESSAGE_MAXSIZE)
        return (errno = EPROTO, 0) ;
    if (!genalloc_readyplus(unsigned int, &b->offsets, 1) ||
        !stralloc_readyplus(&b->data, len))
        return 0 ;
    byte_copy(b->offsets.s + b->offsets.len, sizeof(unsigned int), (char const *)&cur) ;
    b->offsets.len += sizeof(unsigned int) ;
    return 1 ;
}

int bozmessage_put_and_close (bozmessage_sender_t *b, bozmessage_t const *m) {
    if (!reserve_and_copy(b, m->len)) 
        return 0 ;
    byte_copy(b->data.s + b->data.len, m->len, m->s) ;
    b->data.len += m->len ;
    return 1 ;
}

int bozmessage_putv_and_close (bozmessage_sender_t *b, bozmessage_v_t const *m) {
    unsigned int len = 0 ;
    register unsigned int i = 0 ;
    for (; i < m->vlen ; i++) len += m->v[i].len ;
    if (!reserve_and_copy(b, len)) return 0 ;
    for (i = 0 ; i < m->vlen ; i++) {
        byte_copy(b->data.s + b->data.len, m->v[i].len, m->v[i].s) ;
        b->data.len += m->v[i].len ;
    }
    return 1 ;
}
