#include "proto_transfer.h"
#include "sbe_transfer.h"
#include "tlv_transfer.h"
#include "sbe_schema.h"
#include "tlv_schema.h"

proto_transfer_factory::proto_transfer_factory()
{
    transfers_[PT_SBE] = new sbe_transfer;
    transfers_[PT_TLV] = new tlv_transfer;
}

proto_transfer_factory::~proto_transfer_factory()
{
    for (std::map<int, proto_transfer *>::iterator itr = transfers_.begin();
        itr != transfers_.end(); ++itr)
    {
        delete itr->second;
    }
}

int proto_transfer_factory::get_proto_type(const char *service)
{
    const tlv_schema *tlv_schema = tlv_schema_manager::get_instance()->get_schema(service);
    if (tlv_schema != NULL) return PT_TLV;

    const sbe_schema *sbe_schema = sbe_schema_manager::get_instance()->get_schema(service);
    if (sbe_schema != NULL) return PT_SBE;

    return -1;
}

proto_transfer *proto_transfer_factory::get_transfer(const char *service)
{
    int proto_type = get_proto_type(service);
    return get_transfer(proto_type);
}

proto_transfer *proto_transfer_factory::get_transfer(int proto_type)
{
    std::map<int, proto_transfer *>::iterator itr = transfers_.find(proto_type);
    return itr != transfers_.end() ? itr->second : NULL;
}
