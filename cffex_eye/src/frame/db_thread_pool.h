#ifndef DB_HTREAD_POOL_H
#define DB_HTREAD_POOL_H

#include <string>
#include <vector>
#include <map>
#include <cffex/singleton.h>
#include "db_thread.h"

class db_connection;
class db_thread;
class db_command;

class db_thread_pool : public cffex::singleton<db_thread_pool>
{
public:
    db_thread_pool();
    virtual ~db_thread_pool();

    bool init(int thread_num, cffex::event::event_service_thread::event_queue* event_queue);

    // db_name: 数据库连接串 示例 mysql://127.0.0.1[,3306]@test
    //                         oracle://rman_conn
    bool add_connection(int dbid, const std::string &db_name,
        const std::string &user_name, const std::string &password);

    // 启动线程
    void start();

    // 程序启动时插件调用读取配置
    db_connection *get_connection(int dbid);

    // 插件处理完数据后调用写入数据库
    bool push_command(db_command *cmd, bool parallel_write);

private:
    std::vector<db_thread *> threads_;
    cffex::event::event_service_thread::event_queue *event_queue_;
};

#endif // DB_HTREAD_POOL_H
