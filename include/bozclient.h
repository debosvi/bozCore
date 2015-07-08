/* ISC license. */

#ifndef SKACLIENT_H
#define SKACLIENT_H

#include <sys/types.h>
#include <skalibs/kolbak.h>
#include <skalibs/siovec.h>
#include <skalibs/tai.h>
#include <skalibs/uint32.h>

#include <bozCore/bozmessage.h>


 /* Server part */

extern int bozclient_server_ack (bozmessage_t const *, bozmessage_sender_t *, bozmessage_sender_t *, char const *, unsigned int, char const *, unsigned int) ;
extern int bozclient_server_bidi_ack (bozmessage_t const *, bozmessage_sender_t *, bozmessage_sender_t *, bozmessage_receiver_t *, char *, unsigned int, char *, unsigned int, char const *, unsigned int, char const *, unsigned int) ;
extern int bozclient_server_init (bozmessage_receiver_t *, bozmessage_sender_t *, bozmessage_sender_t *, char const *, unsigned int, char const *, unsigned int, tain_t const *, tain_t *) ;
#define bozclient_server_init_g(in, out, asyncout, before, beforelen, after, afterlen, deadline) bozclient_server_init(in, out, asyncout, before, beforelen, after, afterlen, (deadline), &STAMP)
#define bozclient_server_01x_init(before, beforelen, after, afterlen, deadline, stamp) bozclient_server_init(bozmessage_receiver_0, bozmessage_sender_1, bozmessage_sender_x, before, beforelen, after, afterlen, deadline, stamp)
#define bozclient_server_01x_init_g(before, beforelen, after, afterlen, deadline) bozclient_server_01x_init(before, beforelen, after, afterlen, (deadline), &STAMP)


 /* Client part: the rest of this file */

#define BOZCLIENT_KOLBAK_SIZE 8

#define bozclient_buffer_type(bufsn, bufan, qlen) struct { char bufs[bufsn] ; char bufa[bufan] ; kolbak_closure_t q[qlen] ; }
typedef bozclient_buffer_type(BOZMESSAGE_BUFSIZE, BOZMESSAGE_BUFSIZE, BOZCLIENT_KOLBAK_SIZE) bozclient_buffer_t, *bozclient_buffer_t_ref ;


 /* User structure */

typedef struct bozclient_s bozclient_t, *bozclient_t_ref ;
struct bozclient_s {
  bozmessage_receiver_t syncin ;
  bozmessage_sender_t syncout ;
  kolbak_queue_t kq ;
  bozmessage_receiver_t asyncin ;
  bozmessage_sender_t asyncout ;
} ;
#define BOZCLIENT_ZERO { .syncin = BOZMESSAGE_RECEIVER_ZERO, .syncout = BOZMESSAGE_SENDER_ZERO, .kq = KOLBAK_QUEUE_ZERO, .asyncin = BOZMESSAGE_RECEIVER_ZERO, .asyncout = BOZMESSAGE_SENDER_ZERO }
extern bozclient_t const bozclient_zero ;


 /* Callback data for init */

typedef struct bozclient_cbdata_s bozclient_cbdata_t, *bozclient_cbdata_t_ref ;
struct bozclient_cbdata_s
{
  bozclient_t *a ;
  char const *after ;
  unsigned int afterlen ;
} ;


 /* Starting and ending */

extern void bozclient_end (bozclient_t *) ;

extern int bozclient_start_async (bozclient_t *, char *, unsigned int, char *, unsigned int, kolbak_closure_t *, unsigned int, const int, char const *, unsigned int, char const *, unsigned int, bozclient_cbdata_t *) ;
#define bozclient_start_async_b(a, sb, fd, before, beforelen, after, afterlen, blah) bozclient_start_async(a, (sb)->bufs, sizeof((sb)->bufs), (sb)->bufa, sizeof((sb)->bufa), (sb)->q, sizeof((sb)->q), fd, before, beforelen, after, afterlen, blah)

extern int bozclient_start (bozclient_t *, char *, unsigned int, char *, unsigned int, kolbak_closure_t *, unsigned int, const int, char const *, unsigned int, char const *, unsigned int, tain_t const *, tain_t *) ;
#define bozclient_start_b(a, sb, fd, before, beforelen, after, afterlen, deadline, stamp) bozclient_start(a, (sb)->bufs, sizeof((sb)->bufs), (sb)->bufa, sizeof((sb)->bufa), (sb)->q, sizeof((sb)->q) / sizeof(kolbak_closure_t), fd, before, beforelen, after, afterlen, deadline, stamp)
#define bozclient_start_g(a, bufs, bufsn, bufa, bufan, q, qlen, fd, before, beforelen, after, afterlen, deadline)  bozclient_start(a, bufs, bufsn, bufa, bufan, q, qlen, fd, before, beforelen, after, afterlen, (deadline), &STAMP)
#define bozclient_start_b_g(a, sb, fd, before, beforelen, after, afterlen, deadline) bozclient_start_b(a, sb, fd, before, beforelen, after, afterlen, (deadline), &STAMP)


 /* Writing */

extern int bozclient_putmsg_and_close (bozclient_t *, bozmessage_t const *, bozmessage_handler_func_t *, void *) ;
#define bozclient_putmsg(a, m, cb, result) bozclient_putmsg_and_close(a, m, cb, result)
extern int bozclient_putmsgv_and_close (bozclient_t *, bozmessage_v_t const *, bozmessage_handler_func_t *, void *) ;
#define bozclient_putmsgv(a, m, cb, result) bozclient_putmsgv_and_close(a, m, cb, result)

extern int bozclient_put (bozclient_t *, char const *, unsigned int, bozmessage_handler_func_t *, void *) ;
extern int bozclient_putv (bozclient_t *, siovec_t const *, unsigned int, bozmessage_handler_func_t *, void *) ;


 /* Writing and flushing */

extern int bozclient_sendmsg_and_close (bozclient_t *, bozmessage_t const *, bozmessage_handler_func_t *, void *, tain_t const *, tain_t *) ;
#define bozclient_sendmsg_and_close_g(a, m, cb, result, deadline) bozclient_sendmsg_and_close(a, m, cb, result, (deadline), &STAMP)
#define bozclient_sendmsg(a, m, cb, result, deadline, stamp) bozclient_sendmsg_and_close(a, m, cb, result, deadline, stamp)
#define bozclient_sendmsg_g(a, m, cb, result, deadline) bozclient_sendmsg(a, m, cb, result, (deadline), &STAMP)

extern int bozclient_sendmsgv_and_close (bozclient_t *, bozmessage_v_t const *, bozmessage_handler_func_t *, void *, tain_t const *, tain_t *) ;
#define bozclient_sendmsgv_and_close_g(a, m, cb, result, deadline) bozclient_sendmsgv_and_close(a, m, cb, result, (deadline), &STAMP)
#define bozclient_sendmsgv(a, m, cb, result, deadline, stamp) bozclient_sendmsgv_and_close(a, m, cb, result, deadline, stamp)
#define bozclient_sendmsgv_g(a, m, cb, result, deadline) bozclient_sendmsgv(a, m, cb, result, (deadline), &STAMP)

extern int bozclient_send (bozclient_t *, char const *, unsigned int, bozmessage_handler_func_t *, void *, tain_t const *, tain_t *) ;
#define bozclient_send_g(a, s, len, cb, result, deadline) bozclient_send(a, s, len, cb, result, (deadline), &STAMP)
extern int bozclient_sendv (bozclient_t *, siovec_t const *, unsigned int, bozmessage_handler_func_t *, void *, tain_t const *, tain_t *) ;
#define bozclient_sendv_g(a, v, vlen, cb, result, deadline) bozclient_sendv(a, v, vlen, cb, result, (deadline), &STAMP)


 /* Helpers for full async */

#define bozclient_sfd(a) bozmessage_receiver_fd(&(a)->syncin)
#define bozclient_siswritable(a) (!bozmessage_sender_isempty(&(a)->syncout))
#define bozclient_flush(a) bozmessage_sender_flush(&(a)->syncout)
#define bozclient_timed_flush(a, deadline, stamp) bozmessage_sender_timed_flush(&(a)->syncout, deadline, stamp)
#define bozclient_timed_flush_g(a, deadline) bozclient_timed_flush(a, (deadline), &STAMP)

#define bozclient_supdate(a) bozmessage_handle(&(a)->syncin, (bozmessage_handler_func_t *)&kolbak_call, &(a)->kq)
#define bozclient_timed_supdate(a, deadline, stamp) bozmessage_timed_handle(&(a)->syncin, (bozmessage_handler_func_t *)&kolbak_call, &(a)->kq, deadline, stamp)
#define bozclient_timed_supdate_g(a, deadline) bozclient_timed_supdate(a, (deadline), &STAMP)

extern int bozclient_syncify (bozclient_t *, tain_t const *, tain_t *) ;

#define bozclient_fd(a) bozclient_afd(a)
#define bozclient_afd(a) bozmessage_receiver_fd(&(a)->asyncin)
#define bozclient_update(a, f, p) bozclient_aupdate(a, f, p)
#define bozclient_aupdate(a, f, p) bozmessage_handle(&(a)->asyncin, f, p)

extern bozmessage_handler_func_t bozclient_default_cb ;


 /* When asyncout is actually used (skabus...) */

#define bozclient_aiswritable(a) (!bozmessage_sender_isempty(&(a)->asyncout))
#define bozclient_aput_and_close(a, m, bits) bozmessage_put_and_close(&(a)->asyncout, m, bits)
#define bozclient_aputv_and_close(a, m, bits) bozmessage_putv_and_close(&(a)->asyncout, m, bits)
#define bozclient_aput(a, m) bozmessage_put(&(a)->asyncout, m)
#define bozclient_aputv(a, m) bozmessage_putv(&(a)->asyncout, m)
#define bozclient_aflush(a) bozmessage_sender_flush(&(a)->asyncout)

#endif
