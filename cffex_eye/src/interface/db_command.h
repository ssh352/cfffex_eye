#ifndef DB_COMMAND_H
#define DB_COMMAND_H

#include <string>
#include <sqlapi/SQLAPI.h>

class db_command
{
public:
    virtual ~db_command() {}
    // ʵ�����ݿ�Ĳ����߼�
    virtual int get_dbid() = 0;
    // return false if SAException occured
    virtual bool on_execute(SAConnection *connection, int &db_error_out) = 0;
    virtual void release() = 0;
};

#endif // DB_COMMAND_H
