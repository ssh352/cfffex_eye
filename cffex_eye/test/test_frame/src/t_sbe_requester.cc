#include <gtest/gtest.h>

#include <cffex/net/requester.h>
#include "xp_parser.h"
/*
//sbe header
#include "MessageHeader.h"
#include "GetCar.h"
#include "Car.h"
using namespace example;

#define SERVICE_ID_GETCAR 8005

typedef cffex::net::requester<xp_decoder, xp_encoder> REQUESTER_TYPE;

class sbe_requester_handler
{
public:
	sbe_requester_handler(cffex::asio::io_service &io_service, REQUESTER_TYPE *req){
        timer_request_.init(io_service.get_timer_manager(), 1000,
            boost::bind(&requester_handler::do_request, this),
            cffex::event::thread_timer::TIMER_ONCE);
        timer_request_.start();

        req_ = req;
        seqno_ = 0;
        req->set_read_callback(cffex::bind(&requester_handler::on_read, this, cffex::_1, cffex::_2, cffex::_3, cffex::_4, cffex::_5));
	}
	~sbe_requester_handler(){};

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
		en_.init(XP_REQUEST);
		en_.set_serviceid(SERVICE_ID_GETCAR);
		en_.set_seqno(seqno_);
		GetCar getcar;

		//set head
		MessageHeader header;
		header.wrap(en_.get_buffer()->top(), 0, 0, en_.get_buffer()->capacity())
			.blockLength(getcar::sbeBlockLength())
			.templateId(getcar::sbeTemplateId())
			.schemaId(getcar::sbeSchemaId())
			.version(getcar::sbeSchemaVersion());
		getcar.wrapForEncode(en_.get_buffer()->top(), header.encodedLength(), en_.get_buffer()->capacity())
			.param1(1)
			.param2(2)
			.param3(std::string("my_param3"))
			.param4(std::string("my_param4"));
		en_.get_buffer()->inc_loc(header.encodedLength() + getcar.encodedLength());

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

TEST(sbe_request, test_name)
{
	cffex::asio::io_service io_service;
	REQUESTER_TYPE *req = new REQUESTER_TYPE(io_service);

    req->register_addr("tcp://127.0.0.1:31110");
	sbe_requester_handler h(io_service, req);

    io_service.run();
}
*/