#ifndef TLV_TRANSFER_H
#define TLV_TRANSFER_H

#include "proto_transfer.h"

class tlv_transfer : public proto_transfer
{
public:
    virtual ~tlv_transfer() {}
    virtual int http_to_xp(const char *service, const char *message,
        const std::map<std::string, std::string> &params, xp_encoder *e);
    virtual int xp_to_http(xp_decoder *d, cffex::http::http_encoder *e);
};

#endif // TLV_TRANSFER_H
