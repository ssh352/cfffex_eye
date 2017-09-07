#include "mdbpub_api.h"
#include <cffex/bind.h>

mdbpub_api::mdbpub_api(cffex::asio::io_service &io_service, const char *flowpath)
: io_service_(io_service), connector_(io_service_), tcp_session_(NULL), user_flow_(NULL)
{
    connector_.set_tcp_callback(cffex::bind(&mdbpub_api::on_connect_finished, this,
        cffex::_1, cffex::_2, cffex::_3));

    timer_reconnect_.init(io_service_.get_timer_manager(), 3000,
        boost::bind(&mdbpub_api::do_tcp_connect, this),
        cffex::event::thread_timer::TIMER_ONCE);

    if(flowpath != NULL && *flowpath != '\0')
    {
        user_flow_ = new mdb_user_flow(flowpath, "mdbflow");
    }
}

mdbpub_api::~mdbpub_api()
{
    timer_reconnect_.stop();
    if(tcp_session_ != NULL)
        delete tcp_session_;
    if(user_flow_ != NULL)
        delete user_flow_;
}

void mdbpub_api::register_target(const char *tcp_addr, int topic_id)
{
    cffex::asio::endpoint local;
    int type = target_endpoint_.init(tcp_addr, &local);
    if(type != cffex::asio::endpoint::TCP) {
        XLOG(XLOG_ERROR, "mdbpub_api::%s invalid endpoint type[%d]!\n", type);
        return;
    }

    if(topic_id != TOPIC_TENLEVELDATA && topic_id != TOPIC_MEMDB) {
        XLOG(XLOG_ERROR, "mdbpub_api::%s invalid topic_id[%d]!\n", topic_id);
        return;
    }
    topic_ = topic_id;

    XLOG(XLOG_DEBUG, "mdbpub_api::%s ep[%s] topic[%d]\n",
        __FUNCTION__, target_endpoint_.to_string().c_str(), topic_);
    io_service_.post(boost::bind(&mdbpub_api::do_tcp_connect, this));
}

void mdbpub_api::on_connect_finished(const cffex::asio::socket_handle &fd,
                        const cffex::asio::endpoint &target,
                        const cffex::system::error_code &ec)
{
    if(ec)
    {
        XLOG(XLOG_ERROR, "mdbpub_api::%s error occurred! ec[%d]\n", __FUNCTION__, ec);
        timer_reconnect_.start();
        return;
    }

    XLOG(XLOG_DEBUG, "mdbpub_api::%s connect fd[%d] ec[%d]\n",
        __FUNCTION__, fd, ec);

    tcp_session_ = new cffex::ptrade::xtp_cpr_xmp_session(io_service_, fd);
    tcp_session_->set_read_callback(cffex::bind(&mdbpub_api::on_read, this, cffex::_1, cffex::_2, cffex::_3));
    tcp_session_->set_close_callback(cffex::bind(&mdbpub_api::on_close, this, cffex::_1, cffex::_2));

    tcp_session_->set_time_interval(3); //todo
    io_service_.post(boost::bind(&mdbpub_api::do_login, this));
}

void mdbpub_api::on_read(cffex::net::session *session, cffex::ptrade::xtp_decoder *d, const cffex::asio::endpoint &ep)
{
    static int count = 0;
    XLOG(XLOG_DEBUG, "mdbpub_api::%s recv tid[0X%08X] count[%d] seqno[%u]\n", __FUNCTION__, d->get_xtp_tid(), ++count, d->get_xtp_seqno());

    if(user_flow_ != NULL)
    {
        uint32_t tid = d->get_xtp_tid();
        if(tid == cffex::ptrade::TID_RApiLoginRsp)
        {
            cffex::ptrade::CApiLoginField field;
            if(d->get_field(field) < 0)
                XLOG(XLOG_WARNING, "mdbpub_api::%s no CApiLoginField found!\n", __FUNCTION__);
            else if(field.CurrDate != user_flow_->get_trading_day())
            {
                //update tradingday
                user_flow_->set_trading_day(field.CurrDate.get_value());
            }
        }
        else if(tid == cffex::ptrade::TID_RtnTenLevelData)
        {
            user_flow_->set_count(d->get_xtp_seqno());
        }
    }

    xtp_callback_(d);
}

void mdbpub_api::on_close(cffex::net::session *session, const cffex::asio::endpoint &ep)
{
    XLOG(XLOG_DEBUG, "mdbpub_api::%s try to reconnect ...\n", __FUNCTION__);
    timer_reconnect_.start();
}

void mdbpub_api::do_tcp_connect()
{
    XLOG(XLOG_DEBUG, "mdbpub_api::%s try to connect ep[%s]\n", __FUNCTION__, target_endpoint_.to_string().c_str());
    connector_.connect(cffex::asio::endpoint::TCP, target_endpoint_, 3);
}

void mdbpub_api::do_login()
{
    XLOG(XLOG_DEBUG, "mdbpub_api::%s \n", __FUNCTION__);

    cffex::ptrade::CApiLoginField field;

    if(user_flow_ != NULL)
    {
        field.SequenceNo = user_flow_->get_count();
        field.CurrDate.set_value(user_flow_->get_trading_day());
    }

    XLOG(XLOG_DEBUG, "mdbpub_api::%s login field seqno[%d] tradingday[%s]\n",
        __FUNCTION__, field.SequenceNo.get_value(), field.CurrDate.get_value());

    if(topic_ == TOPIC_MEMDB)
        encoder_.init(cffex::ptrade::XTP_PUBLISH, cffex::ptrade::TID_RWdbLogin);
    else
        encoder_.init(cffex::ptrade::XTP_PUBLISH, cffex::ptrade::TID_RApiLoginReq);

    encoder_.set_field(field);
    encoder_.encode();

    tcp_session_->send(encoder_.get_buf(), encoder_.get_len());
}
