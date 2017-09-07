#include "thread_manager.h"
#include "service_thread.h"
#include <boost/bind.hpp>
#include "sbe_schema.h"
#include "tlv_schema.h"

bool thread_manager::load_proto_config()
{
    XLOG(XLOG_DEBUG, "thread_manager::%s \n", __FUNCTION__);
    return sbe_schema_manager::get_instance()->load("./sbe_schema") &&
        tlv_schema_manager::get_instance()->load("./tlv_schema");
}

void thread_manager::start_thread(uint32_t thread_num)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s thread_num[%d]\n", __FUNCTION__, thread_num);
    if (thread_num == 0) thread_num = 1;
    if (thread_num > 64) thread_num = 64;

    for (uint32_t i = 0; i < thread_num; ++i)
    {
        service_thread *thread = new service_thread(*this);
        thread->start();
        threads_.push_back(thread);
    }
}

void thread_manager::add_requester(const std::vector<uint16_t> &services, const std::vector<std::string> &addrs)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s\n", __FUNCTION__);
    for (uint32_t i = 0; i < threads_.size(); ++i)
    {
        threads_[i]->get_io_service()->post(boost::bind(&service_thread::add_requester, threads_[i], services, addrs));
    }
}

bool thread_manager::start_http_server(const char *addr)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s addr[%s]\n", __FUNCTION__, addr);
    std::vector<cffex::asio::io_service *> v;
    for (uint32_t i = 0; i < threads_.size(); ++i)
    {
        v.push_back(threads_[i]->get_io_service());
    }

    http_server_ = new cffex::http::http_server(v);
    for (uint32_t i = 0; i < threads_.size(); ++i)
    {
        http_server_->set_read_callback(i, boost::bind(&service_thread::on_http_request, threads_[i], _1, _2, _3, _4));
    }

    if (http_server_->register_addr(addr) != 0)
    {
        return false;
    }

    return true;
}

int thread_manager::http_response(uint32_t container_id, uint32_t session_id, uint64_t seqno, const char *buf, int len)
{
    return http_server_->send_response(container_id, session_id, seqno, buf, len);
}
