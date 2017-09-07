#ifndef PROT_TRANSFER_H
#define PROT_TRANSFER_H

#include "xp_parser.h"
#include <cffex/http/http_server.h>
#include <cffex/singleton.h>

struct proto_transfer
{
    virtual ~proto_transfer() {}
    virtual int http_to_xp(const char *service, const char *message,
        const std::map<std::string, std::string> &params, xp_encoder *e) = 0;
    virtual int xp_to_http(xp_decoder *d, cffex::http::http_encoder *e) = 0;
};

class proto_transfer_factory : public cffex::singleton<proto_transfer_factory>
{
public:
    proto_transfer_factory();
    ~proto_transfer_factory();

    proto_transfer *get_transfer(const char *service);
    proto_transfer *get_transfer(int proto_type);

private:
    int get_proto_type(const char *service);
    std::map<int, proto_transfer *> transfers_;
};

#endif // PROT_TRANSFER_H
