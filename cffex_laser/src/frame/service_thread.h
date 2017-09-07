#ifndef SERVICE_THREAD_H
#define SERVICE_THREAD_H

#include <cffex/asio/io_service.h>
#include <cffex/system/thread.h>
#include <cffex/http/http_server.h>
#include <cffex/net/requester.h>
#include "xp_parser.h"
#include <map>
#include <vector>

class thread_manager;

class service_thread
{
    typedef cffex::net::requester<xp_decoder, xp_encoder> requester_type;
public:
    service_thread(thread_manager &manager);

    void start();
    void stop();

    void add_requester(const std::vector<uint16_t> &services, const std::vector<std::string> &addrs);

    void on_http_request(uint32_t container_id, uint32_t sessionid, cffex::http::http_decoder *d, const cffex::asio::endpoint &ep);
    void on_xp_response(const cffex::system::error_code &code, uint32_t seqno,
                                    xp_decoder *d, const cffex::asio::endpoint &ep, void *context);

    cffex::asio::io_service *get_io_service() { return &io_service_; }

private:
    int send_xp_request(uint32_t seqno, const char *buf, int len, void *context = NULL);

    int http_response(uint32_t container_id, uint32_t session_id, uint64_t seqno, const char *buf, int len);

    int error_http_response(uint32_t container_id, uint32_t session_id, uint64_t seqno, int error_code);

    requester_type *get_requester(uint16_t service_id);

private:
    thread_manager &manager_;
    cffex::system::thread thread_;
    cffex::asio::io_service io_service_;

    std::vector<requester_type *> requesters_;

    struct requester_container
    {
        std::vector<requester_type *> requesters;
        std::vector<requester_type *>::iterator iter;
    };
    std::vector<requester_container *> containers_;
    std::map<uint16_t, requester_container *> requesters_by_service_;

    cffex::http::http_encoder http_encoder_;
    xp_encoder xp_encoder_;
};

#endif // SERVICE_THREAD_H
