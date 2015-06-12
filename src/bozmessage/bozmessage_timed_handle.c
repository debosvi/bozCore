/* ISC license. */

#include <skalibs/functypes.h>
#include <skalibs/tai.h>
#include <skalibs/unix-timed.h>
#include <bozCore/bozmessage.h>

typedef struct bozmessage_handler_blah_s bozmessage_handler_blah_t, *bozmessage_handler_blah_t_ref ;
struct bozmessage_handler_blah_s {
    bozmessage_receiver_t *b ;
    bozmessage_handler_func_t *f ;
    void *p ;
};

static int getfd (bozmessage_handler_blah_t *blah) {
    return bozmessage_receiver_fd(blah->b) ;
}

static int get (bozmessage_handler_blah_t *blah) {
    return bozmessage_handle(blah->b, blah->f, blah->p) ;
}

int bozmessage_timed_handle (bozmessage_receiver_t *b, bozmessage_handler_func_t *f, void *p, tain_t const *deadline, tain_t *stamp) {
    bozmessage_handler_blah_t blah = { .b = b, .f = f, .p = p } ;
    return timed_get(&blah, (initfunc_t_ref)&getfd, (initfunc_t_ref)&get, deadline, stamp) ;
}
