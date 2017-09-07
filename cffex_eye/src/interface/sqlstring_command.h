#ifndef SQLSTRING_COMMAND_H
#define SQLSTRING_COMMAND_H

#include <string>
#include "db_command.h"

// ¼òµ¥µÄÖ´ÐÐsqlÓï¾ä
class sqlstring_command : public db_command
{
public:
    sqlstring_command(int dbid, const std::string &sql);

    virtual int get_dbid();
    virtual bool on_execute(SAConnection *connection, int &db_error_out);
    virtual void release();

private:
    int dbid_;
    std::string sql_;
};

#endif // SQLSTRING_COMMAND_H
