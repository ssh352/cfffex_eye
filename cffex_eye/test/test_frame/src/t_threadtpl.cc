#include <gtest/gtest.h>
#include <cffex/log/loghelper.h>
#include <cffex/event/timer_manager.h>
#include <cffex/event/thread_timer.h>
#include "threadtpl.hpp"
#include <stdio.h>
#include "iplugin.h"

class plugin_test
{
public:
	plugin_test(int id) :id_(id) {}
	void init() { XLOG(XLOG_DEBUG, "%s , [%d]\n", __FUNCTION__, id_); }
	void do_task() { XLOG(XLOG_DEBUG, "%s, [%d]\n", __FUNCTION__, id_); }
	void set_id(int num) {
		num_ = num;
		XLOG(XLOG_DEBUG, "%s, [%d]\n", __FUNCTION__, id_);
	}
	void assign_request(request_info *request)
	{
		XLOG(XLOG_DEBUG, "%s, [%d]  [%s]\n", __FUNCTION__, id_, request->data.buff);
	}

	char *p;
private:
	int num_;
	int id_;
};

typedef threadtpl<plugin_test, 3> my_thread;

void stop_thrdtpl(bool *bRun)
{
	*bRun = false;
	XLOG(XLOG_DEBUG, "%s done\n", __FUNCTION__);
}

TEST(threadtpl, c1)
{
	my_thread thdtpl;
	plugin_test *p_t1 = new plugin_test(1);
	plugin_test *p_t2 = new plugin_test(2);
	thdtpl.push_plugin(p_t1);
	thdtpl.push_plugin(p_t2);
	request_info *request = new request_info();
	request_info *request2 = new request_info();
//	request->data.buff = (char *)malloc(20);
	request->data.buff = new char[20];
	strcpy(request->data.buff, "request 1");
	request->data.len = sizeof(request->data.buff);
	request->so_id = 0;

//	request2->data.buff = (char*)malloc(20);
	request2->data.buff = new char[20];
	strcpy(request2->data.buff, "request 2");
	request2->data.len = sizeof(request->data.buff);
	request2->so_id = 1;
	thdtpl.start();

	thdtpl.post(service_request_event::ID, (void*)request);
	thdtpl.post(service_request_event::ID, (void*)request2);


	bool bRun = true;
	cffex::event::timer_manager timer_manager;
	cffex::event::thread_timer timer;
	timer.init(&timer_manager, 2000, cffex::bind(stop_thrdtpl, &bRun), cffex::event::thread_timer::TIMER_ONCE);
	timer.start();

	while(bRun)
	{
		timer_manager.detect_timer_list();
		sleep(1);
	}

	thdtpl.stop();
	thdtpl.join();
}