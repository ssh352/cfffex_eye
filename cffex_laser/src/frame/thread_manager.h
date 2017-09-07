#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <cffex/log/loghelper.h>
#include <cffex/http/http_server.h>
#include <cffex/singleton.h>
#include <vector>

class service_thread;

class thread_manager : public cffex::singleton<thread_manager>
{
public:
    thread_manager() : http_server_(0) {}

    bool load_proto_config();

    void start_thread(uint32_t thread_num);


    void add_requester(const std::vector<uint16_t> &services, const std::vector<std::string> &addrs);
    bool start_http_server(const char *addr);

    int http_response(uint32_t container_id, uint32_t session_id, uint64_t seqno, const char *buf, int len);

private:
    cffex::http::http_server *http_server_;
    std::vector<service_thread *> threads_;
};

#endif // THREAD_MANAGER_H
