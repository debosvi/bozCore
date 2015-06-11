/* ISC license. */

#ifndef _BOZMESSAGE_INTERNAL_H_
#define _BOZMESSAGE_INTERNAL_H_

#include <skalibs/uint16.h>

#define PUT_HDR_MAGIC       ((uint16)0xF7A5)
#define PUT_HDR_MAGIC_SIZE  (sizeof(uint16))
#define PUT_HDR_LEN_SIZE    (sizeof(uint16))
#define PUT_HDR_TOTAL_SIZE  (PUT_HDR_MAGIC_SIZE + PUT_HDR_LEN_SIZE)
#define PUT_HASH_SIZE       (16)

#endif /* _BOZMESSAGE_INTERNAL_H_ */
