#ifndef THREADTPL_HEADER
#define THREADTPL_HEADER

#include <cffex/event/event_service_thread.h>
#include <cffex/event/thread_timer.h>
#include <cffex/log/loghelper.h>
#include <cffex/bind.h>
#include <vector>
#include "iplugin.h"
#include <cffex/event/event.h>

//定义服务请求ID
struct service_request_event : public cffex::event::event_creator_type<service_request_event, 0X0001>{};

//插件管理线程 模板类
//可管理不同类型大的插件
//插件需要提供接口：set_id、init、do_task、assign_request
template <typename PLUGIN, int PLUGIN_NUM=1>
class threadtpl : public cffex::event::event_service_thread {
	typedef cffex::event::event_service_thread base_type;
	typedef threadtpl<PLUGIN, PLUGIN_NUM> this_type;
public:
	threadtpl();
	virtual ~threadtpl(){};

	void start_in_thread();
	void deal(void *data) ;

	int push_plugin(PLUGIN *plugin);

private:
	void do_tasks();

private:
//	std::vector<PLUGIN> plugins_;
	PLUGIN* plugins_[PLUGIN_NUM];
	int cur_plugin_num_;

};

template<typename PLUGIN, int PLUGIN_NUM>
threadtpl<PLUGIN, PLUGIN_NUM>::threadtpl():cur_plugin_num_(0)
{
	//注册
	set_start_in_thread_func(cffex::bind(&this_type::start_in_thread, this));
	//注册服务请求处理函数
	register_func(service_request_event::ID, cffex::bind(&this_type::deal, this, cffex::_1));
}

template<typename PLUGIN, int PLUGIN_NUM>
int threadtpl<PLUGIN, PLUGIN_NUM>::push_plugin(PLUGIN *plugin)
{
//	plugins_.push_back(plugin);
	if (cur_plugin_num_ < PLUGIN_NUM)
	{
		plugins_[cur_plugin_num_] = plugin;
		plugins_[cur_plugin_num_]->set_id(cur_plugin_num_);
		cur_plugin_num_++;
		return 0;
	}
	else
	{
		XLOG(XLOG_ERROR, "push_plugin put more plugin\n");
		return -1;
	}
}

template <typename PLUGIN, int PLUGIN_NUM>
void threadtpl<PLUGIN, PLUGIN_NUM>::start_in_thread()
{
//	for (std::vector<PLUGIN>::iterator i = plugins_.begin(); i != plugins_.end(); ++i)
//	{
//		i->init();
//	}
	for (int i = 0; i < cur_plugin_num_; ++i)
	{
		plugins_[i]->init();
	}

	base_type::register_do_task_function(cffex::bind(&this_type::do_tasks, this));
}

template <typename PLUGIN, int PLUGIN_NUM>
void threadtpl<PLUGIN, PLUGIN_NUM>::do_tasks()
{
//	for (std::vector<PLUGIN>::iterator i = plugins_.begin(); i != plugins_.end(); ++i)
//	{
//		i->do_task();
//	}
	for (int i = 0; i < cur_plugin_num_; ++i)
	{
		plugins_[i]->do_task();
	}
}

template <typename PLUGIN, int PLUGIN_NUM>
void threadtpl<PLUGIN, PLUGIN_NUM>::deal(void *data)
{
	XLOG(XLOG_DEBUG, "threadtpl::%s \n", __FUNCTION__);
	request_info *request = (request_info*)data;
	plugins_[request->so_id]->assign_request(request);

	delete [] request->data.buff;
	delete request;
}



#endif