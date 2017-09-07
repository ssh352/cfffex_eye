#include "db_connection.h"
#include <iostream>
#include <cffex/log/loghelper.h>

db_connection::db_connection()
    : db_type_(SA_Oracle_Client) // 默认值
{
}

db_connection::~db_connection()
{
    if (is_connect())
    {
        conn_.Disconnect();
    }
}

bool db_connection::connect(const std::string &db_name, const std::string &user_name, const std::string &password)
{
    // 只需要支持oracle和mysql，默认oracle
    db_name_ = db_name;
    if (db_name_.find("oracle://") == 0)
    {
        db_type_ = SA_Oracle_Client;
        db_name_ = db_name_.substr(strlen("oracle://"));
    }
    else if (db_name_.find("mysql://") == 0)
    {
        db_type_ = SA_MySQL_Client;
        db_name_ = db_name_.substr(strlen("mysql://"));
    }
    user_name_ = user_name;
    password_ = password;

    return connect();
}

bool db_connection::connect()
{
    XLOG(XLOG_DEBUG, "db_connection::%s\n", __FUNCTION__);
    try
    {
        //建立数据库连接
        conn_.Connect(db_name_.c_str(), user_name_.c_str(), password_.c_str(), (SAClient_t)db_type_);
        conn_.setAutoCommit(SA_AutoCommitOn);
    }
    catch (SAException &x)
    {
        XLOG(XLOG_ERROR, "db_connection::%s connect database failed, errorid=%d, msg=%s\n",
            __FUNCTION__, x.ErrNativeCode(), (const char*)x.ErrText());

        try
        {
            check_connection_error(x.ErrNativeCode());
            rollback();
        }
        catch (SAException &)
        {
        }
        return false;
    }
    return true;
}

bool db_connection::is_connect()
{
    return conn_.isConnected();
}

bool db_connection::reconnect()
{
    XLOG(XLOG_INFO, "db_connection::%s try to reconnect Database\n", __FUNCTION__);
    conn_.Disconnect();
    connect();

    return is_connect();
}

void db_connection::commit()
{
    XLOG(XLOG_DEBUG, "db_connection::%s\n", __FUNCTION__);
    conn_.Commit();
}

void db_connection::rollback()
{
    XLOG(XLOG_DEBUG, "db_connection::%s\n", __FUNCTION__);
    conn_.Rollback();
}

bool db_connection::check_connection_error(int err_code)
{
    if (db_type_ == SA_Oracle_Client)
    {
        const int ORACLE_CONNECTION_ERROR_CODE[] =
        {
            0, 12500, 1034, 3113, 3114, 12571, 1033, 1012, 28
        };

        for (unsigned i = 0; i < sizeof(ORACLE_CONNECTION_ERROR_CODE) / sizeof(int); i++)
        {
            if (err_code == ORACLE_CONNECTION_ERROR_CODE[i])
            {
                XLOG(XLOG_INFO, "db_connection::%s Disconnect, error[%d]\n", __FUNCTION__, err_code);
                return true;
            }
        }
    }
    else if (db_type_ == SA_MySQL_Client)
    {
        const int MYSQL_CONNECTION_ERROR_CODE[] =
        {
            0, 2006, 2012, 2013, 2014
        };

        for (unsigned i = 0; i < sizeof(MYSQL_CONNECTION_ERROR_CODE) / sizeof(int); i++)
        {
            if (err_code == MYSQL_CONNECTION_ERROR_CODE[i])
            {
                XLOG(XLOG_INFO, "db_connection::%s Disconnect, error[%d]\n", __FUNCTION__, err_code);
                return true;
            }
        }
    }

    return false;
}
