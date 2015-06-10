/* ISC license. */

#include <skalibs/sysdeps.h>
#include <skalibs/nonposix.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <skalibs/uint16.h>
#include <skalibs/uint32.h>
#include <skalibs/bytestr.h>
#include <skalibs/diuint.h>
#include <skalibs/allreadwrite.h>
#include <skalibs/genalloc.h>
#include <skalibs/djbunix.h>
#include <bozCore/bozmessage.h>

#define MSG_MAGIC ((unsigned int)0xA55A)

int bozmessage_sender_flush (bozmessage_sender_t *b)
{
  unsigned int last = b->data.len ;
  unsigned int *offsets = genalloc_s(unsigned int, &b->offsets) ;
  unsigned int n = genalloc_len(unsigned int, &b->offsets) ;
  register int r = 0 ;

  if (b->shorty) /* we had a short write, gotta send the remainder first */
  {
    unsigned int *next = b->head+1 < n ? offsets + b->head+1 : &last ;
    unsigned int len = next - offsets[b->head] ;
    if (b->shorty <= len)
      r = fd_write(b->fd, b->data.s + offsets[b->head] + (len - b->shorty), b->shorty) ;
    if (r <= 0) return 0 ;
    b->shorty -= r ;
    if (b->shorty) return (errno = EWOULDBLOCK, 0) ;
  }

  for (; b->head < n ; b->head++)
  {
    unsigned int *next = b->head+1 < n ? offsets + b->head+1 : &last ;
    unsigned int len = next->left - offsets[b->head].left ;
    unsigned int nfds = next->right - offsets[b->head].right ;
    char pack[6] ;
    struct iovec v[2] =
    {
      { .iov_base = pack, .iov_len = 6 },
      { .iov_base = b->data.s + offsets[b->head].left, .iov_len = len }
    } ;
    char ancilbuf[CMSG_SPACE(nfds * sizeof(int))] ;
    struct msghdr hdr =
    {
      .msg_name = 0,
      .msg_namelen = 0,
      .msg_iov = v,
      .msg_iovlen = 2,
      .msg_control = nfds ? ancilbuf : 0,
      .msg_controllen = nfds ? sizeof(ancilbuf) : 0
    } ;
    uint32_pack_big(pack, (uint32)len) ;
    uint16_pack_big(pack + 4, (uint16)nfds) ;
    if (nfds)
    {
      struct cmsghdr *cp = CMSG_FIRSTHDR(&hdr) ;
      register unsigned int i = 0 ;
      byte_zero(ancilbuf, sizeof(ancilbuf)) ;
      cp->cmsg_level = SOL_SOCKET ;
      cp->cmsg_type = SCM_RIGHTS ;
      cp->cmsg_len = CMSG_LEN(nfds * sizeof(int)) ;
      for (; i < nfds ; i++)
      {
        register int fd = genalloc_s(int, &b->fds)[offsets[b->head].right + i] ;
        ((int *)CMSG_DATA(cp))[i] = fd < 0 ? -(fd+1) : fd ;
      }
    }
    do r = sendmsg(b->fd, &hdr, MSG_NOSIGNAL) ;
    while (r < 0 && errno == EINTR) ;
    if (r <= 0) return 0 ;
#ifndef SKALIBS_HASANCILAUTOCLOSE
    if (nfds)
    {
      register unsigned int i = 0 ;
      for (; i < nfds ; i++)
      {
        register int fd = genalloc_s(int, &b->fds)[offsets[b->head].right + i] ;
        if (fd < 0) fd_close(-(fd+1)) ;
      }
    }
#endif
    if ((unsigned int)r < 6 + len)
    {
      b->shorty = 6 + len - r ;
      return (errno = EWOULDBLOCK, 0) ;
    }
  }
  b->data.len = 0 ;
  genalloc_setlen(int, &b->fds, 0) ;
  genalloc_setlen(unsigned int, &b->offsets, 0) ;
  b->head = 0 ;
  return 1 ;
}
