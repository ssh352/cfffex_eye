#include "db_thread.h"
#include "db_connection.h"
#include "db_command.h"
#include <cffex/bind.h>
#include <iostream>
#include <cffex/log/loghelper.h>
#include "db_thread_pool.h"
#include <cffex/atomic.h>
#include <cffex/event/event.h>

db_thread::db_thread(cffex::event::event_service_thread::event_queue* public_queue)
    : private_queue_(102400)
{
    register_queue(public_queue);
    register_func(DB_COMMAND_MSG, cffex::bind(&db_thread::do_command_msg, this, cffex::_1));
}

db_thread::~db_thread()
{
    for (std::map<int, db_connection *>::iterator itr = connections_.begin();
    itr != connections_.end(); ++itr)
    {
        delete itr->second;
    }
}

bool db_thread::add_connection(int dbid, const std::string &db_name, const std::string &user_name, const std::string &password)
{
    XLOG(XLOG_DEBUG, "db_thread::%s dbid[%d] db_name[%s]\n", __FUNCTION__, dbid, db_name.c_str());
    db_connection *c = new db_connection;
    if (!c->connect(db_name, user_name, password))
    {
        delete c;
        return false;
    }

    connections_.insert(std::make_pair(dbid, c));

    return true;
}

db_connection *db_thread::get_connection(int dbid)
{
    std::map<int, db_connection *>::iterator itr = connections_.find(dbid);
    if (itr == connections_.end() || !itr->second->is_connect())
    {
        return NULL;
    }
    return itr->second;
}

void db_thread::do_command_msg(void *data)
{
    db_command *command = (db_command *)data;
    std::map<int, db_connection *>::iterator itr = connections_.find(command->get_dbid());
    if (itr == connections_.end() || !itr->second->is_connect())
    {
        XLOG(XLOG_WARNING, "db_thread::%s connection not found, dbid[%d]\n", __FUNCTION__, command->get_dbid());
        command->release();
        return;
    }
    if (!itr->second->is_connect())
    {
        XLOG(XLOG_INFO, "db_thread::%s connection disconnected, dbid[%d]\n", __FUNCTION__, command->get_dbid());
        itr->second->connect();
        if (!itr->second->is_connect())
        {
            XLOG(XLOG_WARNING, "db_thread::%s connection disconnected, dbid[%d]\n", __FUNCTION__, command->get_dbid());
            command->release();
            return;
        }
    }
    int db_error = 0;
    if (!command->on_execute(*itr->second, db_error))
    {
        // maybe disconnect dabase
        if (itr->second->check_connection_error(db_error))
        {
            // retry
            itr->second->reconnect();
            db_error = 0;
            command->on_execute(*itr->second, db_error);
        }
    }

    command->release();
}
