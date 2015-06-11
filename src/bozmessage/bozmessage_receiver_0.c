/* ISC license. */

/* MT-unsafe */

#include <bozCore/bozmessage.h>

static char mainbuf[BOZMESSAGE_BUFSIZE] ;

bozmessage_receiver_t bozmessage_receiver_0_ = BOZMESSAGE_RECEIVER_INIT(0, mainbuf, BOZMESSAGE_BUFSIZE) ;
