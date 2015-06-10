/* ISC license. */

#include <skalibs/functypes.h>
#include <skalibs/tai.h>
#include <skalibs/unix-timed.h>
#include <bozCore/bozmessage.h>

static int bozmessage_sender_isnonempty (bozmessage_sender_t *b) {
    return !bozmessage_sender_isempty(b) ;
}

int bozmessage_sender_timed_flush (bozmessage_sender_t *b, tain_t const *deadline, tain_t *stamp) {
    return timed_flush(b, 
        (initfunc_t_ref)&bozmessage_sender_getfd, 
        (initfunc_t_ref)&bozmessage_sender_isnonempty, 
        (initfunc_t_ref)&bozmessage_sender_flush, 
        deadline, stamp) ;
}
