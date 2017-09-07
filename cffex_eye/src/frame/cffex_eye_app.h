#ifndef CFFEX_EYE_APP_H
#define CFFEX_EYE_APP_H

#include "iplugin.h"
#include <cffex/singleton.h>
#include "db_command.h"

class cffex_eye_app : public cffex::singleton<cffex_eye_app>, public iplugin_host
{
protected:
    virtual SAConnection *get_connection(int dbid);
    // 报警数据入库
    virtual bool post_sql_command(db_command *cmd, bool parallel_write = true);

    virtual bool post_respond(buffer_trans data, rsp_session_info session_info);
};

#endif // CFFEX_EYE_APP_H
