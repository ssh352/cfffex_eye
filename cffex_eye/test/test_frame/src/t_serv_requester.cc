#include <gtest/gtest.h>

#include <cffex/net/requester.h>
#include "xp_parser.h"


typedef cffex::net::requester<xp_decoder, xp_encoder> REQUESTER_TYPE;

class requester_handler
{
public:
	requester_handler(cffex::asio::io_service &io_service, REQUESTER_TYPE *req){
        timer_request_.init(io_service.get_timer_manager(), 1000,
            boost::bind(&requester_handler::do_request, this),
            cffex::event::thread_timer::TIMER_ONCE);
        timer_request_.start();

        req_ = req;
        seqno_ = 0;
        req->set_read_callback(cffex::bind(&requester_handler::on_read, this, cffex::_1, cffex::_2, cffex::_3, cffex::_4, cffex::_5));
	}
	~requester_handler(){};

	void on_read(const cffex::system::error_code &code,
                    uint32_t seqno,
                    xp_decoder *d,
                    const cffex::asio::endpoint &ep,
                    void *context){
		XLOG(XLOG_DEBUG, "requester_handler::%s code[%d] seqno[%d]\n",
			__FUNCTION__, code, seqno);

		if (code == 0)
		{
			d->dump();
			XLOG(XLOG_DEBUG, "requester_handler::%s body[%s]\n", __FUNCTION__, d->get_body());
		}
	}

	void do_request(){
		const char* data = "hello xp_protocol request";
		en_.init(XP_REQUEST);
		en_.set_seqno(seqno_);
		en_.set_messageid(2);
		en_.set_resultcode(3);
		en_.set_serviceid(8001);
		en_.set_version(5);
		en_.set_proto_type(6);
		en_.set_buf(data, strlen(data));
		req_->send_request(seqno_, en_.get_buf(), en_.get_len());
		seqno_++;
	}


private:
	cffex::event::thread_timer  timer_request_;
	REQUESTER_TYPE *req_;
	uint32_t seqno_;
	xp_encoder en_;
};

TEST(ser_request, test_name)
{
	cffex::asio::io_service io_service;
	REQUESTER_TYPE *req = new REQUESTER_TYPE(io_service);

    req->register_addr("tcp://127.0.0.1:31110");
	requester_handler h(io_service, req);

    io_service.run();
}