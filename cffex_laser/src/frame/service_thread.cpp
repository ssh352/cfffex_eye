#include "service_thread.h"
#include <boost/bind.hpp>
#include <iostream>
#include <cffex/log/loghelper.h>
#include <thread_manager.h>
#include <sys/syscall.h>
#include "proto_transfer.h"
#include <cffex/common/string_helper.h>
#include <cffex/http/url_coder.h>
#include "error_info.h"
#include "jsoncoder.h"

#define gettid() syscall(__NR_gettid)

service_thread::service_thread(thread_manager &manager)
    : manager_(manager)
{
}

void service_thread::start()
{
    thread_.start(boost::bind(&cffex::asio::io_service::run, &io_service_, 100));
}

void service_thread::stop()
{
    for (auto itr = requesters_.begin(); itr != requesters_.end(); ++itr)
    {
        delete *itr;
    }

    for (auto itr = containers_.begin(); itr != containers_.end(); ++itr)
    {
        delete *itr;
    }

    io_service_.stop();
    thread_.join();
}

void service_thread::add_requester(const std::vector<uint16_t> &services, const std::vector<std::string> &addrs)
{
    if (services.size()  == 0 || addrs.size() == 0)
    {
        XLOG(XLOG_INFO, "service_thread::%s services or addrs empty\n", __FUNCTION__);
        return;
    }

    requester_container *container = new requester_container;

    for (auto itr = addrs.begin(); itr != addrs.end(); ++itr)
    {
        requester_type *r = new requester_type(io_service_);
        r->set_read_callback(boost::bind(&service_thread::on_xp_response, this, _1, _2, _3, _4, _5));
        r->register_addr(itr->c_str());
        requesters_.push_back(r);
        container->requesters.push_back(r);
    }

    container->iter = container->requesters.begin();
    for (auto itr = services.begin(); itr != services.end(); ++itr)
    {
        requesters_by_service_[*itr] = container;
    }
    containers_.push_back(container);
}

void service_thread::on_http_request(uint32_t container_id, uint32_t sessionid, cffex::http::http_decoder *d, const cffex::asio::endpoint &ep)
{
    XLOG(XLOG_DEBUG, "service_thread::%s path[%s]\n", __FUNCTION__, d->url_path());
    int error_code = 0;

    std::string path = d->url_path();
    size_t pos = path.find("?");
    if (pos == std::string::npos)
    {
        XLOG(XLOG_ERROR, "service_thread::%s get params failed, path[%s]\n", __FUNCTION__, d->url_path());
        error_http_response(container_id, sessionid, d->get_seqno(), ERROR_BAD_REQUEST);
        return;
    }
    std::string params = path.substr(pos + 1);
    path = path.substr(0, pos);

    std::vector<std::string> path_vec;
    cffex::common::string_helper::split(path, '/', &path_vec);
    if (path_vec.size() != 5 || (path_vec[1] + "/" + path_vec[2]) != "cffexLaser/service")
    {
        XLOG(XLOG_ERROR, "service_thread::%s check path failed, path[%s]\n", __FUNCTION__, d->url_path());
        error_http_response(container_id, sessionid, d->get_seqno(), ERROR_BAD_REQUEST);
        return;
    }

    std::vector<std::string> params_vec;
    cffex::common::string_helper::split(params, '&', &params_vec);

    std::map<std::string, std::string> params_map;
    for (auto itr = params_vec.begin(); itr != params_vec.end(); ++itr)
    {
        const std::string &param = *itr;
        pos = param.find("=");
        if (pos == std::string::npos)
        {
            continue;
        }
        std::string name = param.substr(0, pos);
        std::string value = param.substr(pos + 1);
        params_map.insert(std::make_pair(name, cffex::http::url_decode(value.c_str(), value.length())));
    }

    proto_transfer *transfer = proto_transfer_factory::get_instance()->get_transfer(path_vec[3].c_str());
    xp_encoder_.init(XP_REQUEST);
    if ((error_code = transfer->http_to_xp(path_vec[3].c_str(), path_vec[4].c_str(), params_map, &xp_encoder_)) != 0)
    {
        XLOG(XLOG_ERROR, "service_thread::%s http_to_xp transfer failed, path[%s]\n", __FUNCTION__, d->url_path());
        error_http_response(container_id, sessionid, d->get_seqno(), error_code);
        return;
    }

    uint64_t context = ((((uint64_t)container_id) << 32) | sessionid);
    xp_encoder_.set_seqno(d->get_seqno());
    if ((error_code = send_xp_request(d->get_seqno(), xp_encoder_.get_buf(), xp_encoder_.get_len(), (void *)context)) != 0)
    {
        XLOG(XLOG_ERROR, "service_thread::%s send_xp_request failed, path[%s]\n", __FUNCTION__, d->url_path());
        error_http_response(container_id, sessionid, d->get_seqno(), error_code);
        return;
    }
}

int service_thread::send_xp_request(uint32_t seqno, const char *buf, int len, void *context)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s seqno[%d], bufflen[%d]\n", __FUNCTION__, seqno, len);

    xp_header *header = (xp_header *)buf;
    uint16_t service_id = g_ntohs(header->service_id);

    requester_type *requester = get_requester(service_id);
    if (requester == NULL)
    {
        XLOG(XLOG_ERROR, "service_thread::%s get_requester failed, service_id[%d]\n", __FUNCTION__, service_id);
        return ERROR_NO_ALIVE_SERVICE;
    }

    if (requester->send_request(seqno, buf, len, context) != 0)
    {
        XLOG(XLOG_ERROR, "service_thread::%s send xp request failed, service_id[%d]\n", __FUNCTION__, service_id);
        return ERROR_SEND_REQUEST;
    }

    return 0;
}

void service_thread::on_xp_response(const cffex::system::error_code &code, uint32_t seqno,
                                xp_decoder *d, const cffex::asio::endpoint &ep, void *context)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s seqno[%d]\n", __FUNCTION__, seqno);

    uint64_t ucontext = (uint64_t)context;
    uint32_t container_id = ucontext >> 32;
    uint32_t sessionid = ucontext;
    int error_code = 0;

    if (code || d == NULL)
    {
        XLOG(XLOG_ERROR, "service_thread::%s error_code[%d:%s], seqno[%d]\n", __FUNCTION__, code,
                                            cffex::system::error::get_error_msg(code), seqno);
        error_http_response(container_id, sessionid, seqno, code);
        return;
    }

    proto_transfer *transfer = proto_transfer_factory::get_instance()->get_transfer(d->get_proto_type());
    http_encoder_.init(200);
    http_encoder_.head("Content-Type", "text/html");
    if ((error_code = transfer->xp_to_http(d, &http_encoder_)) != 0)
    {
        XLOG(XLOG_ERROR, "service_thread::%s xp_to_http failed, seqno[%d], code[%d]\n", __FUNCTION__, seqno, error_code);
        error_http_response(container_id, sessionid, d->get_seqno(), error_code);
        return;
    }

    http_response(container_id, sessionid, d->get_seqno(), http_encoder_.get_buf(), http_encoder_.get_len());
}

int service_thread::http_response(uint32_t container_id, uint32_t session_id, uint64_t seqno, const char *buf, int len)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s seqno[%d]\n", __FUNCTION__, seqno);
    return manager_.http_response(container_id, session_id, seqno, buf, len);
}

int service_thread::error_http_response(uint32_t container_id, uint32_t session_id, uint64_t seqno, int error_code)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s seqno[%d]\n", __FUNCTION__, seqno);

    http_encoder_.init(200);
    http_encoder_.head("Content-Type", "text/html");
    json_encoder json_root(true);

    json_root.set_value("version", 0);
    json_root.set_value("errorCode", error_code);

    json_root.set_value("errorMsg", std::string(error_code >= 100000 ? get_error_message(error_code)
                                                                     : cffex::system::error::get_error_msg(error_code)));

    http_encoder_.body(json_root.to_string());
    http_response(container_id, session_id, seqno, http_encoder_.get_buf(), http_encoder_.get_len());
    return 0;
}

service_thread::requester_type *service_thread::get_requester(uint16_t service_id)
{
    XLOG(XLOG_DEBUG, "thread_manager::%s service_id[%d]\n", __FUNCTION__, service_id);

    std::map<uint16_t, requester_container *>::iterator iter = requesters_by_service_.find(service_id);
    if (iter == requesters_by_service_.end())
    {
        return NULL;
    }

    service_thread::requester_type *ret = NULL;
    size_t try_count = iter->second->requesters.size();
    while (try_count--)
    {
        if ((*iter->second->iter)->is_ready())
        {
            ret = *iter->second->iter;
            try_count = 0;
        }

        ++iter->second->iter;
        if (iter->second->iter == iter->second->requesters.end())
        {
            iter->second->iter = iter->second->requesters.begin();
        }
    }

    return ret;
}
