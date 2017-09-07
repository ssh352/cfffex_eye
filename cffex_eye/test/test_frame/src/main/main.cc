#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <stdexcept>

#include <cffex/log/loghelper.h>
#include "gtest/gtest.h"

#include <cffex/log/loghelper.h>
#include <cffex/platform.h>
#include "db_thread.h"
#include "sqlstring_command.h"
#include <iostream>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

static const int max_argc = 16;
static const int argv_buf_len = 128;
int g_argc = 0;
char *g_argv[max_argc];

void sig_ignore(int n) {
    //fprintf(stdout, "%s, signal[%d]\n", __FUNCTION__, n);
}
void sig_exit(int n) {
    fprintf(stdout, "%s, signal[%d]\n", __FUNCTION__, n);
    exit(0);
}

int main(int argc, char **argv)
{
    /*
    cffex::event::event_service_thread::msg_queue msg_queue;
    db_thread t(&msg_queue);
    t.add_connection(0, "mysql://127.0.0.1@test", "root", "root@123");
    t.start();
    while (1)
    {
        std::cout << "press any key to post sql command" << std::endl;
        std::cin.get();
        sqlstring_command *cmd = new sqlstring_command(0, "insert into t_test(id, name, pass, age) values(1, 'xupei', 'pass', 100)");
        t.post_event(cmd);
    }
    //*/

    fprintf(stdout, "---- __cplusplus[%ld], CFFEX_IS_LITTLE_ENDIAN[%d]----- \n",
        __cplusplus, CFFEX_IS_LITTLE_ENDIAN);
    #ifdef __GNUC__
    fprintf(stdout, "---- GNU[%d]----- \n", __GNUC__);
    #endif

    //SET_LOG_FILE("./log/all.log");
    SET_LOG_LEVEL("all");

    cffex::signal(SIGPIPE, sig_ignore);
    cffex::signal(SIGINT,  sig_exit);
    cffex::signal(SIGBUS, sig_ignore);

    if(argc > max_argc) {
        throw std::runtime_error("too much options");
    }
    g_argc = argc;
    for(int i = 0; i < argc; i++) {
        g_argv[i] = new char[argv_buf_len];
        memcpy(*(g_argv+i), *(argv+i), argv_buf_len);
    }

    int ret  =  0;
    try {
        InitGoogleTest(&argc, argv);
        ret = RUN_ALL_TESTS();
    } catch(std::runtime_error e) {
        fprintf(stdout, "exception[%s]\n", e.what());
    }

#ifdef WIN32
    system("pause");
#endif

    return ret;

}




