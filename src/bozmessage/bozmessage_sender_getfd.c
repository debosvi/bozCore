/* ISC license. */

#include <bozCore/bozmessage.h>

int bozmessage_sender_getfd (bozmessage_sender_t const *b) {
    return b->fd ;
}
