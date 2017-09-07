#ifndef SERV_RESPONSER_H
#define SERV_RESPONSER_H

#include "xp_parser.h"
#include <cffex/net/responser.h>
#include <cffex/system/thread.h>
#include <vector>
#include <cffex/singleton.h>
#include "so_service.hpp"
#include "iplugin.h"

//默认使用req
typedef cffex::net::responser<xp_decoder, xp_encoder> responser_type;

const int SERVICE_THREAD = 2;

class serv_responser : public cffex::singleton<serv_responser>
{
public:

	~serv_responser(){
		delete responser_;
	}

	bool init(const char* serv_addr, so_service *services)
	{
		services_ = services;
		for (int i = 0; i < SERVICE_THREAD; ++i)
		{
			vec_io_services_.push_back(new cffex::asio::io_service());
			vec_thread_.push_back(new cffex::system::thread());
			vec_thread_[i]->start(cffex::bind(&cffex::asio::io_service::run, vec_io_services_[i], 100));
		}

		responser_ = new responser_type(vec_io_services_);
		responser_->register_addr(serv_addr);
		responser_->set_read_callback(cffex::bind(&serv_responser::on_read, this,
            cffex::_1, cffex::_2, cffex::_3, cffex::_4));

		return true;
	}

    void on_read(   uint32_t containerid,
	                uint32_t sessionid,
	                xp_decoder *decoder,
	                const cffex::asio::endpoint &ep)
    {
    	XLOG(XLOG_DEBUG, "serv_responser::%s \n", __FUNCTION__);
    	//获得请求中的服务号
		request_info *req_copy = new request_info();
		//req_copy->so_id = so_id_; //need to be setted in so_plugin
		req_copy->data.seqno = decoder->get_seqno();
		req_copy->data.len = decoder->get_len();
		req_copy->data.buff = new char[req_copy->data.len];	//copy data to another thread
		memcpy(req_copy->data.buff, decoder->get_buf(), req_copy->data.len);
		req_copy->session_info.containerid = containerid;
		req_copy->session_info.sessionid = sessionid;

		int service_id = decoder->get_serviceid();
		so_plugin* plugin = services_->get_service_plugin(service_id);
		if( plugin != NULL)
			plugin->send_request(req_copy);
    }

    cffex::asio::io_service* get_container_io_service(uint32_t containerid)
    {
    	XLOG(XLOG_DEBUG, "serv_responser::%s \n", __FUNCTION__);
    	return responser_->get_io_service(containerid);
    }

    int send_response(buffer_trans data, rsp_session_info session_info)
    {
    	int ret = responser_->send_response(session_info.containerid, session_info.sessionid, data.seqno, data.buff, data.len);
    	delete[] data.buff;

    	XLOG(XLOG_DEBUG, "serv_responser::%s ret[%d]\n", __FUNCTION__, ret);
    	return ret;
    }

private:
	std::vector<cffex::asio::io_service *> vec_io_services_;
	std::vector<cffex::system::thread *> vec_thread_;
	responser_type *responser_;
	so_service *services_;

};


#endif