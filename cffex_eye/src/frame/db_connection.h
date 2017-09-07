#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <string>
#include "db_command.h"

class db_connection
{
public:
    db_connection();
    virtual ~db_connection();

    bool connect(const std::string &db_name, const std::string &user_name, const std::string &password);
    bool connect();
    bool is_connect();
    bool reconnect();

    bool check_connection_error(int err_code);

    operator SAConnection *() { return &conn_; }
    void commit();
    void rollback();

private:
    SAConnection conn_;
    int db_type_;
    std::string db_name_;
    std::string user_name_;
    std::string password_;
};

#endif // DB_CONNECTION_H
