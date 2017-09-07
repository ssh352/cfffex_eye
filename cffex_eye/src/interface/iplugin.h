#ifndef IPLUGIN_HEAD
#define IPLUGIN_HEAD

#include <cffex/event/event_service_thread.h>
#include <vector>
#include <string>

class SAConnection;
class db_command;

struct buffer_trans
{
	char* buff;
	int len;
	int seqno;
};

struct rsp_session_info
{
	uint32_t containerid;
	uint32_t sessionid;
};

struct request_info
{
	buffer_trans data;   //请求数据
	rsp_session_info session_info; //请求对应的session信息
	int so_id;	//业务处理流程使用，标记是线程中那个so处理这个请求
};


struct iplugin_host
{
	virtual ~iplugin_host() {}
	// 只用于启动时加载配置
	virtual SAConnection *get_connection(int dbid) = 0;
	// 报警数据入库
	virtual bool post_sql_command(db_command *cmd, bool parallel_write = true) = 0;
	// 发送请求应答
	virtual bool post_respond(buffer_trans data, rsp_session_info session_info) = 0;
};

struct iplugin
{
	virtual ~iplugin() {}
	virtual std::vector<int> get_service_id() = 0;
	virtual int init_so(iplugin_host *host) = 0;
	virtual void regist_subscribe_topic(std::vector<uint16_t> &sub_topics) = 0;
	virtual int assign_task(uint16_t fid, char const *data) = 0;
	virtual int assign_request(request_info *req_info) = 0;

};

#endif
