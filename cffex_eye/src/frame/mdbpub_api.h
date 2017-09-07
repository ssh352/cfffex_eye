#ifndef CFFEX_MDBPUB_API_H
#define CFFEX_MDBPUB_API_H

#include <cffex/net/stream_connector.h>
#include <cffex/ptrade/field/types_define.h>
#include <cffex/ptrade/field/xtp_field.h>
#include <cffex/ptrade/field/xtp_fieldid.h>
#include <cffex/ptrade/field/xtp_tid.h>
#include <cffex/ptrade/session/xtp_cpr_xmp_session.h>
#include <cffex/ptrade/parser/xtp_parser.h>
#include <cffex/event/thread_timer.h>
#include "mdbflow.h"

class mdbpub_api
{
    typedef cffex::function<void (cffex::ptrade::xtp_decoder *)> xtp_callback;
public:
    enum { TOPIC_TENLEVELDATA = 0x1, TOPIC_MEMDB = 0x2 };

    mdbpub_api(cffex::asio::io_service &io_service, const char *flowname = NULL);
    ~mdbpub_api();

    void register_target(const char *tcp_addr, int topic_id);
    void set_heartbeat_interval() {}

    template <typename FUNC>
    void set_xtp_callback(FUNC f) {
        xtp_callback_ = f;
    }

private:
    void do_tcp_connect();
    void do_login();

    // connector callback
    void on_connect_finished(const cffex::asio::socket_handle &fd,
                        const cffex::asio::endpoint &target,
                        const cffex::system::error_code &ec);

    // session callback
    void on_read(cffex::net::session *session, cffex::ptrade::xtp_decoder *d, const cffex::asio::endpoint &ep);
    void on_close(cffex::net::session *session, const cffex::asio::endpoint &ep);

private:
    int topic_;
    cffex::asio::io_service &io_service_;
    cffex::net::stream_connector connector_;
    cffex::event::thread_timer timer_reconnect_;
    cffex::asio::endpoint target_endpoint_;
    cffex::ptrade::xtp_cpr_xmp_session *tcp_session_;
    cffex::ptrade::xtp_compress_xmp_encoder encoder_;

    xtp_callback xtp_callback_;
    mdb_user_flow *user_flow_;
};

#endif