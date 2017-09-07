#ifndef CFFEX_MDBPUB_ADAPTER_H
#define CFFEX_MDBPUB_ADAPTER_H

#include <cffex/fib/publisher.h>
#include <cffex/bind.h>
#include <cffex/ptrade/field/field_tag.h>
#include <cffex/ptrade/parser/xtp_parser.h>
#include <cffex/flow/cache_flow.h>
#include <cffex/flow/file_flow.h>
#include <cffex/system/thread.h>
#include "mdbpub_api.h"
#include "fibsub_api.h"

class mdbpub_adapter
{
public:
    mdbpub_adapter(cffex::cache_flow &xtp_flow)
        : mdbpub_api_(NULL)
        , fibsub_api_(NULL)
        , xtp_flow_(xtp_flow)
    {
    }

    bool init_mdbpub_api(const char *addr, int topic, const char *flow_path)
    {
        mdbpub_api_ = new mdbpub_api(io_service_, flow_path);
        mdbpub_api_->set_xtp_callback(cffex::bind(&mdbpub_adapter::handle_xtp_msg, this, cffex::_1));
        mdbpub_api_->register_target(addr, topic);
        return true;
    }

    bool inti_fibsub_api(const char *addr, int topic, int subscriber_id, const char *flow_path)
    {
        fibsub_api_ = new fibsub_api(io_service_, xtp_flow_, flow_path);
        fibsub_api_->register_target(addr, topic, subscriber_id);
    }

    ~mdbpub_adapter()
    {
        if (mdbpub_api_ != NULL)
        {
            delete mdbpub_api_;
        }
        if (fibsub_api_ != NULL)
        {
            delete fibsub_api_;
        }
    }

    void start()
    {
        thread_.start(cffex::bind(&cffex::asio::io_service::run, &io_service_, 100));
    }

private:
    void handle_xtp_msg(cffex::ptrade::xtp_decoder *d)
    {
		xtp_flow_.append(d->get_buf(), d->get_len());
    }

private:
    cffex::asio::io_service io_service_;
    mdbpub_api *mdbpub_api_;
    fibsub_api *fibsub_api_;
	cffex::cache_flow &xtp_flow_;
    cffex::system::thread thread_;
};

#endif