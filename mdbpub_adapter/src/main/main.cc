#include "config_manager.h"
#include "mdbpub_adapter.h"
#include <cffex/log/loghelper.h>
#include <cffex/system/thread.h>
#include <cffex/bind.h>
#include <cffex/flow/cache_flow.h>

const char *version = "1.0.0";
const char *config_file = "./mdbpub_adapter.xml";

void show_version(int argc, char *argv[]) {
    const char *p = strrchr(argv[0], '/');
    const char *exe_name = p != NULL ? p + 1 : argv[0];
    printf("%s version: %s\n", exe_name, version);
    printf("Build time: %s %s\n", __TIME__, __DATE__);
}

void ParseArg(int argc, char *argv[]) {
    int ch;
    while(-1 != (ch = getopt(argc, argv, "v"))) {
        switch(ch) {
          case 'v':{
            show_version(argc, argv);
            exit(0);
          }
        }
      }
}

int main(int argc, char **argv)
{
    ParseArg(argc, argv);
    config_manager::get_instance()->parse_config_file(config_file);

    SET_LOG_FILE(config_manager::get_instance()->get_log_file_path());
    SET_LOG_LEVEL(config_manager::get_instance()->get_log_level());

    cffex::cache_flow mdb_xtp_flow;

    cffex::asio::io_service io_service;
    mdbpub_adapter adapter(io_service, mdb_xtp_flow);

    io_service.run();

    return 0;
}
