#include "sqlstring_command.h"
#include <iostream>
#include <cffex/log/loghelper.h>

sqlstring_command::sqlstring_command(int dbid, const std::string &sql)
    : dbid_(dbid)
    , sql_(sql)
{
}

int sqlstring_command::get_dbid()
{
    return dbid_;
}

bool sqlstring_command::on_execute(SAConnection *connection, int &db_error_out)
{
    XLOG(XLOG_DEBUG, "sqlstring_command::%s sql=%s\n", __FUNCTION__, sql_.c_str());
    SACommand cmd(connection);
    try
    {
        cmd.setCommandText(sql_.c_str());
        cmd.Execute();
    }
    catch (SAException &x)
    {
        db_error_out = x.ErrNativeCode();
        XLOG(XLOG_ERROR, "sqlstring_command::%s db execute failed, errorid=%d, msg=%s, sql=%s\n",
            __FUNCTION__, x.ErrNativeCode(), (const char*)x.ErrText(), sql_.c_str());

        try
        {
            connection->Rollback();
        }
        catch (SAException &)
        {
        }
        return false;
    }
    return true;
}

void sqlstring_command::release()
{
    delete this;
}
