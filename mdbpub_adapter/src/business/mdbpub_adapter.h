#ifndef CFFEX_MDBPUB_ADAPTER_H
#define CFFEX_MDBPUB_ADAPTER_H

#include <cffex/fib/publisher.h>
#include <cffex/bind.h>
#include <cffex/ptrade/field/field_tag.h>
#include <cffex/ptrade/parser/xtp_parser.h>
#include <cffex/flow/cache_flow.h>
#include "config_manager.h"
#include "mdbpub_api.h"

#include <cffex/flow/file_flow.h>

class mdbpub_adapter
{
public:
    mdbpub_adapter(cffex::asio::io_service &io_service, cffex::cache_flow &xtp_flow)
    : io_service_(io_service), publisher_(NULL), mdbpub_api_(io_service, config_manager::get_instance()->get_flow_path())
        //,file_flow_("./flow", "testflow")
    {
        publisher_ = new cffex::fib::publisher(config_manager::get_instance()->get_xtp_pub_topic(),
            io_service_);
        publisher_->register_addr(config_manager::get_instance()->get_xtp_pub_addr());

        mdbpub_api_.set_xtp_callback(cffex::bind(&mdbpub_adapter::handle_xtp_msg, this, cffex::_1));
        mdbpub_api_.register_target(config_manager::get_instance()->get_mdbpub_addr(),
            config_manager::get_instance()->get_mdbpub_topic());
    }

    ~mdbpub_adapter()
    {
        if(publisher_ != NULL)
        {
            delete publisher_;
            publisher_ = NULL;
        }
    }

private:
    void handle_xtp_msg(cffex::ptrade::xtp_decoder *d)
    {
        static int count = 0;
        d->field_tag(cffex::ptrade::XTP_FIELD_TAG_1_61);
        uint32_t tid = d->get_xtp_tid();
        XLOG(XLOG_DEBUG, "mdbpub_adapter::%s recv tid[0X%08X] count[%d]\n", __FUNCTION__, tid, ++count);
        cffex::ptrade::field_set::const_iterator itor = d->begin();
        while(itor != d->end()) {
            XLOG(XLOG_DEBUG, "mdbpub_adapter::%s fieldid[0X%08X]\n", __FUNCTION__, itor->first);
            ++itor;
        }

        //file_flow_.append(d->get_buf(), d->get_len());

         publisher_->publish((void *)(d->get_buf()), d->get_len());
    }

private:
    cffex::asio::io_service &io_service_;
    cffex::fib::publisher *publisher_;
    mdbpub_api mdbpub_api_;

    //cffex::file_flow file_flow_;
};

#endif