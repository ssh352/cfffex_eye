#ifndef XP_HEADER_H
#define XP_HEADER_H

#include <stdint.h>

struct xp_header
{
    uint8_t type;
    uint8_t ext_head_len;
    uint8_t version;
    uint8_t proto_type;

    uint16_t service_id;
    uint16_t message_id;

    uint32_t seqno;
    uint32_t body_len;
    uint32_t result_code;
};

#endif // XP_HEADER_H
