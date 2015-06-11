/* ISC license. */

#ifndef BOZMESSAGE_H
#define BOZMESSAGE_H

#include <skalibs/uint16.h>
#include <skalibs/buffer.h>
#include <skalibs/gccattributes.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <skalibs/siovec.h>
#include <skalibs/tai.h>


 /* Message */

typedef struct bozmessage_s bozmessage_t, *bozmessage_t_ref ;
struct bozmessage_s {
    char *s ;
    unsigned int len ;
} ;
#define BOZMESSAGE_ZERO { .s = 0, .len = 0 }
extern bozmessage_t const bozmessage_zero ;

typedef struct bozmessage_v_s bozmessage_v_t, *bozmessage_v_t_ref ;
struct bozmessage_v_s {
    siovec_t *v ;
    unsigned int vlen ;
} ;
#define BOZMESSAGE_V_ZERO { .v = 0, .vlen = 0 }
extern bozmessage_v_t const bozmessage_v_zero ;

#define BOZMESSAGE_MAXSIZE      (32768)
#define BOZMESSAGE_BUFSIZE      (2048)

 /* Sender */

typedef struct bozmessage_sender_s bozmessage_sender_t, *bozmessage_sender_t_ref ;
struct bozmessage_sender_s {
    int fd ;
    stralloc data ;
    genalloc offsets ;      /* unsigned int */
    unsigned int head ;     /* chunk index to send */
    unsigned int shorty ;   /* unsent data length in current chunk */
} ;
#define BOZMESSAGE_SENDER_ZERO BOZMESSAGE_SENDER_INIT(-1)
#define BOZMESSAGE_SENDER_INIT(s) { .fd = (s), .data = STRALLOC_ZERO, .offsets = GENALLOC_ZERO, .head = 0, .shorty = 0 }

extern bozmessage_sender_t const bozmessage_sender_zero ;
extern void bozmessage_sender_init (bozmessage_sender_t *, int) ;
extern void bozmessage_sender_free (bozmessage_sender_t *) ;
#define bozmessage_sender_fd(b) ((b)->fd)
extern int bozmessage_sender_getfd (bozmessage_sender_t const *) gccattr_pure ;
#define bozmessage_sender_isempty(b) (!genalloc_len(unsigned int, &(b)->offsets))

extern int bozmessage_put_and_close (bozmessage_sender_t *, bozmessage_t const *) ;
#define bozmessage_put(b, m) bozmessage_put_and_close(b, m)
extern int bozmessage_putv_and_close (bozmessage_sender_t *, bozmessage_v_t const *) ;
#define bozmessage_putv(b, m) bozmessage_putv_and_close(b, m)

extern int bozmessage_unput (bozmessage_sender_t *) ;

extern int bozmessage_sender_flush (bozmessage_sender_t *) ;
extern int bozmessage_sender_timed_flush (bozmessage_sender_t *, tain_t const *, tain_t *) ;
#define bozmessage_sender_timed_flush_g(sender, deadline) bozmessage_sender_timed_flush(sender, (deadline), &STAMP)


 /* Receiver */

typedef struct bozmessage_receiver_s bozmessage_receiver_t, *bozmessage_receiver_t_ref ;
struct bozmessage_receiver_s {
  buffer_t mainb ;
  stralloc maindata ;
  uint16 mainlen ;
} ;
#define BOZMESSAGE_RECEIVER_ZERO { .mainb = CBUFFER_ZERO, .maindata = STRALLOC_ZERO, .mainlen = 0 }
#define BOZMESSAGE_RECEIVER_INIT(d, mains, mainn) \
{ \
  .mainb = BUFFER_INIT(buffer_read, d, mains, mainn), \
  .maindata = STRALLOC_ZERO, \
  .mainlen = 0 \
}
extern int bozmessage_receiver_init (bozmessage_receiver_t *, int, char *, unsigned int) ;
extern void bozmessage_receiver_free (bozmessage_receiver_t *) ;
#define bozmessage_receiver_fd(b) buffer_fd(&(b)->mainb)
#define bozmessage_receiver_isempty(b) buffer_isempty(&(b)->mainb)
#define bozmessage_receiver_isfull(b) buffer_isfull(&(b)->mainb) 

extern int bozmessage_receive (bozmessage_receiver_t *, bozmessage_t *) ;
extern int bozmessage_timed_receive (bozmessage_receiver_t *, bozmessage_t *, tain_t const *, tain_t *) ;
#define bozmessage_timed_receive_g(receiver, msg, deadline) bozmessage_timed_receive(receiver, msg, (deadline), &STAMP)

typedef int bozmessage_handler_func_t (bozmessage_t const *, void *) ;
typedef bozmessage_handler_func_t *bozmessage_handler_func_t_ref ;

extern int bozmessage_handle (bozmessage_receiver_t *, bozmessage_handler_func_t *, void *) ;
extern int bozmessage_timed_handle (bozmessage_receiver_t *, bozmessage_handler_func_t *, void *, tain_t const *, tain_t *) ;
#define bozmessage_timed_handle_g(b, f, p, deadline) bozmessage_timed_handle(b, f, p, (deadline), &STAMP)


 /* Globals */

extern bozmessage_receiver_t bozmessage_receiver_0_ ;
#define bozmessage_receiver_0 (&bozmessage_receiver_0_)

extern bozmessage_sender_t bozmessage_sender_1_ ;
#define bozmessage_sender_1 (&bozmessage_sender_1_)

extern bozmessage_sender_t bozmessage_sender_x_ ;
#define bozmessage_sender_x (&bozmessage_sender_x_)

#endif
