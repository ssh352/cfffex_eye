#include "mdbpub_adapter.h"
#include <cffex/log/loghelper.h>
#include <cffex/system/thread.h>
#include <cffex/bind.h>
#include <cffex/flow/cache_flow.h>
#include <cffex/common/xml_config_parser.h>
#include <vector>
#include <string>
#include <iostream>
#include <cffex/common/so_loader.h>
#include <cffex/event/event_service_thread.h>
#include "iplugin.h"
#include "threadtpl.hpp"
#include "so_plugin.hpp"
#include "so_service.hpp"
#include "serv_responser.h"
#include "db_thread_pool.h"
#include "db_command.h"
#include <cffex/atomic.h>
#include "cffex_eye_app.h"
#include "version.h"

cffex::atomic<int> db_put_count(0);
cffex::atomic<int> db_exec_count(0);

const char *version = "1.0.0";
const char *config_file = "./cffex_eye.xml";

void show_version(int argc, char *argv[])
{
    const char *p = strrchr(argv[0], '/');
    const char *exe_name = p != NULL ? p + 1 : argv[0];
    printf("%s version: %s\n", exe_name, version);
    printf("build time: %s %s\n", __TIME__, __DATE__);
    printf("git version: %s\n", GIT_VERSION);
}

bool process_arg(int argc, char *argv[])
{
    char c;
    while ((c = getopt(argc, argv, "v")) != -1)
    {
        if (c == 'v')
        {
            show_version(argc, argv);
            return false;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (!process_arg(argc, argv))
        return 0;
	cffex::common::xml_config_parser parser;
	if (parser.parse_file(config_file) < 0)
	{
		XLOG(XLOG_ERROR, "config_manager::%s error_msg[%s]\n", __FUNCTION__, parser.get_error_message().c_str());
	}

 	SET_LOG_FILE(parser.get_parameter("log/filePath").c_str());
 	SET_LOG_LEVEL(parser.get_parameter("log/logLevel").c_str());

    cffex::event::event_service_thread::event_queue output_queue(102400);
    db_thread_pool::get_instance()->init(
        atoi(parser.get_attribute("thread", "databases").c_str()),
        &output_queue);

    std::vector<cffex::common::xml_config_parser::element> vec_db = parser.get_elements("databases/database");
    for (std::vector<cffex::common::xml_config_parser::element>::iterator itr_db = vec_db.begin();
        itr_db != vec_db.end(); ++itr_db)
    {
        if (!db_thread_pool::get_instance()->add_connection(
            atoi(parser.get_attribute("id", NULL, *itr_db).c_str()),
            parser.get_attribute("databaseName", NULL, *itr_db),
            parser.get_attribute("userName", NULL, *itr_db),
            parser.get_attribute("password", NULL, *itr_db)))
        {
            XLOG(XLOG_ERROR, "main db connect failed\n");
            return 0;
        }
    }
    db_thread_pool::get_instance()->start();

    // init mdbpub reader
    std::string mdbpub_from_adapter = parser.get_attribute("fromAdapter", "mdbpub");
    std::string mdbpub_addr = parser.get_parameter("mdbpub/addr");
    std::string mdbpub_topic_str = parser.get_parameter("mdbpub/topic");
    std::string mdbpub_subid_str = parser.get_parameter("mdbpub/subscriberId");
    std::string mdbpub_flow_path = parser.get_parameter("mdbpub/flowPath");

    uint32_t mdbpub_topic;
    sscanf(mdbpub_topic_str.c_str(), "%x", &mdbpub_topic);
    uint32_t mdbpub_subid;
    sscanf(mdbpub_subid_str.c_str(), "%x", &mdbpub_subid);

	cffex::cache_flow mdb_xtp_flow;
    mdbpub_adapter mdbpub_adapter(mdb_xtp_flow);
    if (mdbpub_from_adapter == "true")
    {
        mdbpub_adapter.inti_fibsub_api(mdbpub_addr.c_str(), mdbpub_topic, mdbpub_subid, mdbpub_flow_path.c_str());
    }
    else
    {
        mdbpub_adapter.init_mdbpub_api(mdbpub_addr.c_str(), mdbpub_topic, mdbpub_flow_path.c_str());
    }

printf("===========================\n");
    cffex::common::so_loader<iplugin> biz_sos;
    biz_sos.load("./src/plugin", "*.so");
    biz_sos.dump();

    //cffex::event::event_service_thread::msg_queue output_queue;
    so_service *services = new so_service();
    std::vector<threadtpl<so_plugin> *> threads;
    for (std::vector<cffex::common::so_loader<iplugin>::so>::iterator i = biz_sos.vec_so.begin(); i != biz_sos.vec_so.end(); ++i)
    {
        //printf("soname[%s] \n", i->name.c_str());
        //iplugin *p = i->obj;
        //p->init_so(info);

        iplugin *iplugin = i->obj;
        threadtpl<so_plugin> *thrd = new threadtpl<so_plugin>();
        threads.push_back(thrd);
        so_plugin* sop = new so_plugin(iplugin, &mdb_xtp_flow, thrd);
        services->add_service(sop);
        thrd->push_plugin(sop);
        thrd->start();
    }

    mdbpub_adapter.start();
    serv_responser::get_instance()->init("tcp://127.0.0.1:31110", services);

	while(1)sleep(1);
    //xtp_tran_thread.join();

    return 0;
}


