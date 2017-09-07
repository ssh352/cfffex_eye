#include "fibsub_api.h"

fibsub_api::fibsub_api(cffex::asio::io_service &io_service, cffex::cache_flow &xtp_flow, const char *flowname/* = NULL*/)
    : io_service_(io_service)
    , xtp_flow_(xtp_flow)
{
    
}

fibsub_api::~fibsub_api()
{
    if (sub_ != NULL)
    {
        delete sub_;
    }
}

bool fibsub_api::register_target(const char *addr, int topic, int subscriber_id)
{
    sub_ = new cffex::fib::subscriber(topic, subscriber_id, &xtp_flow_);
    sub_->register_addr(addr, &io_service_);
    return true;
}
