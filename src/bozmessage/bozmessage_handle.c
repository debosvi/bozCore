/* ISC license. */

#include <bozCore/bozmessage.h>

int bozmessage_handle (bozmessage_receiver_t *b, bozmessage_handler_func_t *f, void *p) {
    unsigned int n = BOZMESSAGE_MAXREADS ;
    int count = 0 ;
    while (n--) {
        bozmessage_t m ;
        register int r = bozmessage_receive(b, &m) ;
        if (r < 0) return -1 ;
        if (!r) break ;
        r = (*f)(&m, p) ;
        if (r <= 0) return r-2 ;
        count++ ;
    }
    return count ;
}
