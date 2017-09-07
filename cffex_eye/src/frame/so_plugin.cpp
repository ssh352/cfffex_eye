#include "so_plugin.hpp"
#include <algorithm>
#include "cffex_eye_app.h"
#include "threadtpl.hpp"

void so_plugin::init()
{
	XLOG(XLOG_DEBUG, "so_plugin::%s \n", __FUNCTION__);
	//初始化so基础调用
	plugin_->init_so(cffex_eye_app::get_instance());

	//读取订阅
	plugin_->regist_subscribe_topic(sub_topics_);

	dump();
}

void so_plugin::dump()
{
	for (std::vector<uint16_t>::iterator i = sub_topics_.begin(); i != sub_topics_.end(); ++i)
	{
		XLOG(XLOG_INFO, "sub_topic is [0X%04X]\n", *i);
	}
}

void so_plugin::do_task()
{
	XLOG(XLOG_DEBUG, "so_plugin::%s, so_id[%d] \n", __FUNCTION__, so_id_);
	//读数据
 	if (reader_.next(&xtp_decoder_) <= 0)
		return;

	uint32_t tid = xtp_decoder_.get_xtp_tid();

	if (tid != cffex::ptrade::TID_MemAlter)
		return;

//	xtp_decoder_.dump();

	cffex::ptrade::field_set::const_iterator itor = xtp_decoder_.begin();
	for (; itor != xtp_decoder_.end(); ++itor)
	{
		uint16_t fid = itor->first;

		std::vector<uint16_t>::const_iterator it = std::find(sub_topics_.begin(), sub_topics_.end(), fid);
		if (it == sub_topics_.end())
		{
			continue;
		}
		else
		{
			//分发数据给so
			//so将处理结果放入输出流
			int ret = plugin_->assign_task(fid,itor->second.buf);
			if (ret == 0)
			{ }
		}
	}
}



bool so_plugin::send_request(request_info *req_copy)
{
	XLOG(XLOG_DEBUG, "so_plugin::%s, so_id[%d] \n", __FUNCTION__, so_id_);
	req_copy->so_id = so_id_;
	return e_thread_->post(service_request_event::ID ,req_copy);
}