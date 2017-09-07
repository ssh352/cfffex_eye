#ifndef SBE_SERVER_H
#define SBE_SERVER_H

#include "iplugin.h"
#include <cffex/log/loghelper.h>
#include "xp_parser.h"

#define SERVICE_ID_GETCAR 8005

class sbe_server : public iplugin
{
public:
	virtual std::vector<int> get_service_id()	{
		std::vector<int> v;
		v.push_back(SERVICE_ID_GETCAR);
		return v;
	}
	virtual int init_so(iplugin_host *host) {
		host_ = host;
		return 0;
	}

	virtual void regist_subscribe_topic(std::vector<uint16_t> &sub_topics)
	{
		XLOG(XLOG_DEBUG, "sbe_server::%s \n", __FUNCTION__);
	}

	virtual int assign_task(uint16_t fid, char const *data)
	{
		XLOG(XLOG_DEBUG, "sbe_server::%s \n", __FUNCTION__);
		return 100;
	}

	virtual int assign_request(request_info *req_info);

private:
	iplugin_host *host_;
	xp_encoder en_;
};

#endif