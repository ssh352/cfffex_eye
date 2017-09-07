#ifndef CFFEX_FIBSUB_API_H
#define CFFEX_FIBSUB_API_H

#include <cffex/fib/subscriber.h>
#include <cffex/fib/publisher.h>
#include <cffex/event/thread_timer.h>
#include <cffex/system/thread.h>

class fibsub_api
{
public:
    fibsub_api(cffex::asio::io_service &io_service, cffex::cache_flow &xtp_flow, const char *flowname = NULL);
    ~fibsub_api();

    bool register_target(const char *addr, int topic, int subscriber_id);

private:
    cffex::asio::io_service &io_service_;
    cffex::cache_flow &xtp_flow_;
    cffex::fib::subscriber *sub_;
};

#endif // CFFEX_FIBSUB_API_H
