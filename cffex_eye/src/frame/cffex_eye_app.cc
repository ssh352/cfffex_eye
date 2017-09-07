#include "cffex_eye_app.h"
#include "db_thread_pool.h"
#include "serv_responser.h"

SAConnection *cffex_eye_app::get_connection(int dbid)
{
    db_connection *conn = db_thread_pool::get_instance()->get_connection(dbid);
    if (conn == NULL)
        return NULL;

    SAConnection *ret = (SAConnection*)conn;
    return ret;
}

// 报警数据入库
bool cffex_eye_app::post_sql_command(db_command *cmd, bool parallel_write)
{
	XLOG(XLOG_DEBUG, "cffex_eye_app::%s \n", __FUNCTION__);
    return db_thread_pool::get_instance()->push_command(cmd, parallel_write);
}


bool cffex_eye_app::post_respond(buffer_trans data, rsp_session_info session_info)
{
	XLOG(XLOG_DEBUG, "cffex_eye_app::%s \n", __FUNCTION__);
	cffex::asio::io_service *io_serv = serv_responser::get_instance()->get_container_io_service(session_info.containerid);
	io_serv->post(cffex::bind(&serv_responser::send_response, serv_responser::get_instance(), data, session_info));
	return true;
}