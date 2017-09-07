#include "db_thread_pool.h"
#include "db_thread.h"
#include <cffex/log/loghelper.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

db_thread_pool::db_thread_pool()
{

}

db_thread_pool::~db_thread_pool()
{
    for (std::vector<db_thread *>::iterator itr = threads_.begin(); itr != threads_.end(); ++itr)
    {
        delete *itr;
    }
}

bool db_thread_pool::init(int thread_num, cffex::event::event_service_thread::event_queue* event_queue)
{
    XLOG(XLOG_DEBUG, "db_thread_pool::%s thread_num[%d]\n", __FUNCTION__, thread_num);
    event_queue_ = event_queue;
    if (thread_num <= 0)
    {
        thread_num = 1;
    }

    for (int i = 0; i < thread_num; ++i)
    {
        db_thread *dt = new db_thread(event_queue_);
        threads_.push_back(dt);
    }
    return true;
}

bool db_thread_pool::add_connection(int dbid, const std::string &db_name,
    const std::string &user_name, const std::string &password)
{
    XLOG(XLOG_INFO, "db_thread_pool::%s id[%d], db_name[%s], user_name[%s]\n",
        __FUNCTION__, dbid, db_name.c_str(), user_name.c_str());

    for (std::vector<db_thread *>::iterator itr = threads_.begin(); itr != threads_.end(); ++itr)
    {
        if (!(*itr)->add_connection(dbid, db_name, user_name, password))
        {
            return false;
        }
    }

    return true;
}

void db_thread_pool::start()
{
    XLOG(XLOG_DEBUG, "db_thread_pool::%s\n", __FUNCTION__);
    for (std::vector<db_thread *>::iterator itr = threads_.begin(); itr != threads_.end(); ++itr)
    {
        (*itr)->start();
    }
}

db_connection *db_thread_pool::get_connection(int dbid)
{
    if (threads_.size() == 0)
    {
        return NULL;
    }

    return threads_[0]->get_connection(dbid);
}

bool db_thread_pool::push_command(db_command *cmd, bool parallel_write)
{
    XLOG(XLOG_DEBUG, "db_connection::%s parallel_write[%d]\n", __FUNCTION__, parallel_write);
    if (parallel_write)
    {
        return event_queue_->push(DB_COMMAND_MSG, cmd);
    }
    else
    {
        return threads_[gettid() % threads_.size()]->post(DB_COMMAND_MSG, cmd);
    }
}
