#include <gtest/gtest.h>
#include "db_connection.h"
#include "db_thread_pool.h"
#include "db_command.h"
#include <iostream>

class test_db_command : public db_command
{
public:
    virtual ~test_db_command() {}
    virtual int get_dbid() { return 0; }
    virtual void release() { delete this; }
    virtual bool on_execute(SAConnection *connection, int &db_error_out)
    {
        SACommand cmd(connection);
        try
        {
            std::string sql = "insert into t_test(id, name, pass, age) values(1, 'xupei', 'pass', 100)";
            cmd.setCommandText(sql.c_str());
            cmd.Execute();
        }
        catch (SAException &x)
        {
            std::cout << "on_execute error" << x.ErrNativeCode() << std::endl;
            db_error_out = x.ErrNativeCode();
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
};

TEST(database_test, connection)
{
    db_connection conn;
    bool ret = conn.connect("mysql://127.0.0.1@test", "root", "root@123");
    EXPECT_EQ(ret, true);
    EXPECT_EQ(conn.is_connect(), true);

    //std::cout << "press any key when mysql is restarted" << std::endl;
    //std::cin.get();
    //EXPECT_EQ(conn.is_connect(), false);
    //std::cout << "is connect: " << conn.is_connect() << std::endl;
    //conn.connect();
    //EXPECT_EQ(conn.is_connect(), true);

    test_db_command *cmd = new test_db_command;
    int db_error = 0;
    ret = cmd->on_execute(conn, db_error);
    cmd->release();
    EXPECT_EQ(ret, true);
}
