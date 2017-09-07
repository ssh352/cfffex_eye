#ifndef SBE_TRANSFER_H
#define SBE_TRANSFER_H

#include "proto_transfer.h"

class sbe_transfer : public proto_transfer
{
public:
    virtual ~sbe_transfer() {}
    virtual int http_to_xp(const char *service, const char *message,
        const std::map<std::string, std::string> &params, xp_encoder *e);
    virtual int xp_to_http(xp_decoder *d, cffex::http::http_encoder *e);
};

#endif // SBE_TRANSFER_H
