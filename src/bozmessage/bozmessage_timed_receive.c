/* ISC license. */

#include <skalibs/functypes.h>
#include <skalibs/tai.h>
#include <skalibs/unix-timed.h>
#include <bozCore/bozmessage.h>

typedef struct bozmessage_get_s bozmessage_get_t, *bozmessage_get_t_ref ;
struct bozmessage_get_s {
    bozmessage_receiver_t *b ;
    bozmessage_t *m ;
};

static int getfd (bozmessage_get_t *g) {
    return bozmessage_receiver_fd(g->b) ;
}

static int get (bozmessage_get_t *g) {
    return bozmessage_receive(g->b, g->m) ;
}

int bozmessage_timed_receive (bozmessage_receiver_t *b, bozmessage_t *m, tain_t const *deadline, tain_t *stamp) {
    bozmessage_get_t g = { .b = b, .m = m } ;
    return timed_get(&g, (initfunc_t *)&getfd, (initfunc_t *)&get, deadline, stamp) ;
}
