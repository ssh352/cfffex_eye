#ifndef PLUGIN_H
#define PLUGIN_H

#include "iplugin.h"
#include <cffex/log/loghelper.h>
#include "xp_parser.h"

class pulgin: public iplugin
{
public:
	pulgin(){};
	~pulgin(){};

	virtual std::vector<int> get_service_id()	{
		std::vector<int> v;
		v.push_back(8001);
		v.push_back(8002);
		return v;
	}
	virtual int init_so(iplugin_host *host) {
		host_ = host;
		return 0;
	}
	virtual void regist_subscribe_topic(std::vector<uint16_t> &sub_topics)
	{
		XLOG(XLOG_DEBUG, "plugin::%s \n", __FUNCTION__);
        sub_topics.push_back(cffex::ptrade::XTP_FID_Trade);
	}
	virtual int assign_task(uint16_t fid, char const *data)
	{
		XLOG(XLOG_DEBUG, "plugin::%s \n", __FUNCTION__);
		switch(fid)
		{
			case cffex::ptrade::XTP_FID_Trade:
				cffex::ptrade::CTradeField trade;
				trade.init_from_stream(data);
				trade.dump();
				break;
		}
		return 100;
	}
	virtual int assign_request(request_info *req_info)
	{
		XLOG(XLOG_DEBUG, "plugin::%s \n", __FUNCTION__);
		xp_decoder de;
		de.decode(req_info->data.buff, req_info->data.len);
		XLOG(XLOG_DEBUG, "rec request data [%s]\n", std::string(de.get_body(), de.get_body_len()).c_str());
		XLOG(XLOG_DEBUG, "en = \n%s\n", binary_dump_string(req_info->data.buff, req_info->data.len).c_str());

		const char* str = "hello xp_protocol respond";
		en_.init(XP_RESPONSE);
		en_.set_seqno(de.get_seqno());
		en_.set_messageid(2);
		en_.set_resultcode(0);
		en_.set_serviceid(8001);
		en_.set_version(de.get_version());
		en_.set_proto_type(de.get_proto_type());
		en_.set_buf(str, strlen(str));

		//返回应答
		buffer_trans data;
		char *p = new char[en_.get_len()];
		memcpy(p, en_.get_buf(), en_.get_len());
		data.buff = p;
		data.len = en_.get_len();
		data.seqno = de.get_seqno();
		host_->post_respond(data, req_info->session_info);

		return 100;
	}

private:
	iplugin_host *host_;
	xp_encoder en_;

};

#endif
