#include "thread_manager.h"
#include "proto_transfer.h"
#include <cffex/common/xml_config_parser.h>
#include <string>
#include <cffex/common/string_helper.h>
#include "version.h"

const char *version = "1.0.0";

bool process_arg(int argc, char *argv[])
{
    char c;
    while ((c = getopt(argc, argv, "v")) != -1)
    {
        if (c == 'v')
        {
            {
                const char *p = strrchr(argv[0], '/');
                const char *exe_name = p != NULL ? p + 1 : argv[0];
                printf("%s version: %s\n", exe_name, version);
                printf("build time: %s %s\n", __TIME__, __DATE__);
                printf("git version: %s\n", GIT_VERSION);
            }
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

    cffex::common::xml_config_parser config;
    if (config.parse_file("config.xml") < 0)
    {
        XLOG(XLOG_ERROR, "%s parse_file error_msg[%s]\n", __FUNCTION__, config.get_error_message().c_str());
        return 0;
    }

    //SET_LOG_FILE(config.get_parameter("log/filePath").c_str());
    SET_LOG_LEVEL(config.get_parameter("log/logLevel").c_str());

    int thread_num = config.get_parameter("threadNum", 2);
    std::string http_addr = config.get_parameter("httpAddr");

    XLOG(XLOG_INFO, "http_addr[%s], thread_num[%d]\n", http_addr.c_str(), thread_num);

    if (!thread_manager::get_instance()->load_proto_config())
    {
        return 0;
    }
    thread_manager::get_instance()->start_thread(thread_num);

    std::vector<cffex::common::xml_config_parser::element> services = config.get_elements("service");
    for (std::vector<cffex::common::xml_config_parser::element>::iterator itr = services.begin();
        itr != services.end(); ++itr)
    {
        std::string ids = config.get_attribute("id", NULL, *itr);
        std::vector<std::string> addrs = config.get_parameters("addr", *itr);

        std::vector<std::string > ids_vec;
        cffex::common::string_helper::split(ids, ',', &ids_vec);

        std::vector<uint16_t> idsv;
        for (auto id = ids_vec.begin(); id != ids_vec.end(); ++id)
        {
            idsv.push_back(atoi(id->c_str()));
        }

        std::string str_addrs;
        for (auto addr = addrs.begin(); addr != addrs.end(); ++addr)
        {
            if (!str_addrs.empty()) str_addrs += ",";
            str_addrs += *addr;
        }
        XLOG(XLOG_INFO, "add service ids[%s], addrs[%s]\n", ids.c_str(), str_addrs.c_str());
        thread_manager::get_instance()->add_requester(idsv, addrs);
    }

    thread_manager::get_instance()->start_http_server(http_addr.c_str());

    while (true)
        sleep(1);
    return 0;
}
