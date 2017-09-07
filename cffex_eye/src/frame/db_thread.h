#ifndef DB_THREAD_H
#define DB_THREAD_H

#include <cffex/event/event_service_thread.h>
#include <cffex/event/thread_timer.h>
#include <map>

class db_connection;
class db_command;

#define DB_COMMAND_MSG 1

class db_thread : public cffex::event::event_service_thread
{
public:
    db_thread(cffex::event::event_service_thread::event_queue* public_queue);
    virtual ~db_thread();

    bool add_connection(int dbid, const std::string &db_name, const std::string &user_name, const std::string &password);
    db_connection *get_connection(int dbid);

protected:
    void do_command_msg(void *data);

private:
    std::map<int, db_connection *> connections_;
    cffex::event::event_service_thread::event_queue private_queue_;
};

#endif // DB_THREAD_H
