#ifndef SO_PLUGIN_HEADER
#define SO_PLUGIN_HEADER

#include "iplugin.h"
#include <cffex/flow/flow.h>
#include <cffex/flow/flow_reader.h>
#include <cffex/ptrade/parser/xmp_parser.h>
#include <cffex/ptrade/parser/xtp_parser.h>
#include <cffex/event/event_service_thread.h>
#include <cffex/log/loghelper.h>
#include <cffex/ptrade/field/xtp_fieldid.h>
#include <cffex/ptrade/field/xtp_field.h>
#include <vector>


class so_plugin
{
public:
	so_plugin():plugin_(NULL), xtp_flow_(NULL), e_thread_(NULL) {}
	so_plugin(iplugin *plugin, cffex::flow *xtp_flow,
		cffex::event::event_service_thread *e_thread)
	:plugin_(plugin), xtp_flow_(xtp_flow), e_thread_(e_thread)
	{
		reader_.set_flow(xtp_flow_);
		xtp_flow_->register_notifier(e_thread_);
	}
	virtual ~so_plugin() {}

	void init_default_constructor(iplugin *plugin, cffex::flow *xtp_flow, cffex::event::event_service_thread *e_thread)
	{
		set_plugin(plugin);
		set_flow(xtp_flow);
		set_thread(e_thread);

		reader_.set_flow(xtp_flow_);
		xtp_flow_->register_notifier(e_thread_);
	}
	void set_plugin(iplugin *plugin) { plugin_ = plugin; }
	void set_flow(cffex::flow *xtp_flow) { xtp_flow_ = xtp_flow; }
	void set_thread(cffex::event::event_service_thread *e_thread) { e_thread_ = e_thread; }


	so_plugin& operator=(const so_plugin& sop)
	{
		plugin_			= sop.plugin_;
		xtp_flow_		= sop.xtp_flow_;
		e_thread_ 		= sop.e_thread_;
		reader_.set_flow(xtp_flow_);
		xtp_flow_->register_notifier(e_thread_);
		return *this;
	}

	void dump();

	std::vector<int> get_service_id() { return plugin_->get_service_id(); }

	bool send_request(request_info *copy);

	/** template method */
	void init();
	void do_task();
	void set_id(int id) { so_id_ = id; }
	int assign_request(request_info *req_info) { return plugin_->assign_request(req_info); };


private:
	iplugin *plugin_;
	cffex::flow *xtp_flow_;
	cffex::flow_reader reader_;
	cffex::ptrade::xtp_decoder xtp_decoder_;
	cffex::event::event_service_thread *e_thread_;
	std::vector<uint16_t> sub_topics_;
	int so_id_;  //so_id in threadptl
};


#endif