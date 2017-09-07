#include <stdio.h>
#include <stdlib.h>
#include "iplugin.h"
#include "db_name.h"
#include "sqlstring_command.h"
#include <cffex/ptrade/field/xtp_fieldid.h>
#include <cffex/ptrade/field/xtp_field.h>
#include <cffex/log/loghelper.h>
#include <cffex/ptrade/parser/xmp_parser.h>
#include <cffex/ptrade/parser/xtp_parser.h>
#include <cffex/event/event_service_thread.h>
#include <string>

#define SERVICE_ID_WRITEORDER 9001

class write_order : public iplugin
{
public:
    virtual std::vector<int> get_service_id()
    {
		std::vector<int> v;
		v.push_back(SERVICE_ID_WRITEORDER);
        return v;
    }
	virtual int init_so(iplugin_host *host) {
		XLOG(XLOG_DEBUG, "write_order::%s\n", __FUNCTION__);
        host_ = host;
        return 0;
	}
	virtual void regist_subscribe_topic(std::vector<uint16_t> &sub_topics){
		XLOG(XLOG_DEBUG, "write_order::%s \n", __FUNCTION__);
		sub_topics.push_back(cffex::ptrade::XTP_FID_Order);
//		sub_topics.push_back(cffex::ptrade::XTP_FID_Trade);
//		sub_topics.push_back(cffex::ptrade::FID_RLevelDataDetail);
//		sub_topics.push_back(cffex::ptrade::XTP_FID_CurrentTime);
	}

	virtual int assign_task(uint16_t fid, char const *data) {
		XLOG(XLOG_DEBUG, "write_order::%s, fid[0X%04X]\n", __FUNCTION__, fid);
		switch(fid)
		{
			case cffex::ptrade::XTP_FID_Order:
				cffex::ptrade::COrderField order;
				order.init_from_stream(data);
				std::string sql = "insert into t_order (TradingDay) values('20170714')";
				char szsql[1024] = {0};
				sprintf(szsql, "insert into t_order(TradingDay, SettlementGroupID, SettlementID, OrderSysID, ParticipantID, ClientID, UserID, InstrumentID, OrderPriceType, Direction, CombOffsetFlag, CombHedgeFlag, LimitPrice, VolumeTotalOriginal, TimeCondition, GTDDate, VolumeCondition, MinVolume, ContingentCondition, StopPrice, ForceCloseReason, OrderLocalID, IsAutoSuspend, OrderSource, OrderStatus, OrderType, VolumeTraded, VolumeTotal, InsertDate, InsertTime, ActiveTime, SuspendTime, UpdateTime, CancelTime, ActiveUserID, Priority,  TimeSortID, ClearingPartID, BusinessUnit) values (\'%s\', \'%s\', %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', %d, %d, \'%s\', \'%s\', %f, %d, %d, \'%s\', %d, %d, %d, %f, %d, \'%s\', %d, %d, %d, %d, %d, %d, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', %d, %d, \'%s\', \'%s\')",
					order.TradingDay.get_value(), order.SettlementGroupID.get_value(), order.SettlementID.get_value(), order.OrderSysID.get_value(), order.ParticipantID.get_value(), order.ClientID.get_value(), order.UserID.get_value(), order.InstrumentID.get_value(), order.OrderPriceType.get_value(), order.Direction.get_value(), order.CombOffsetFlag.get_value(), order.CombHedgeFlag.get_value(), order.LimitPrice.get_value(), order.VolumeTotalOriginal.get_value(), order.TimeCondition.get_value(), order.GTDDate.get_value(), order.VolumeCondition.get_value(), order.MinVolume.get_value(), order.ContingentCondition.get_value(), order.StopPrice.get_value(), order.ForceCloseReason.get_value(), order.OrderLocalID.get_value(), order.IsAutoSuspend.get_value(), order.OrderSource.get_value(), order.OrderStatus.get_value(), order.OrderType.get_value(), order.VolumeTraded.get_value(), order.VolumeTotal.get_value(), order.InsertDate.get_value(), order.InsertTime.get_value(), order.ActiveTime.get_value(), order.SuspendTime.get_value(), order.UpdateTime.get_value(), order.CancelTime.get_value(), order.ActiveUserID.get_value(), order.Priority.get_value(), order.TimeSortID.get_value(), order.ClearingPartID.get_value(), order.BusinessUnit.get_value());
                sqlstring_command *cmd = new sqlstring_command(DB_NAME_MYSQL, szsql);
                host_->post_sql_command(cmd);
				break;
		}
        return 0;
	}

	virtual int assign_request(request_info *req_info){return 0;}

private:
    iplugin_host *host_;
};


#ifdef __cplusplus
extern "C"
{
#endif

void* create(){
    write_order *p = new write_order();
	return (void*)p;
}

void destroy(void* p){
	delete (write_order *)p;
}

#ifdef __cplusplus
}
#endif

//g++ -o so1.so so1.cc -shared -fpic -I../frame
//g++ -o so2.so so1.cc -shared -fpic -I../frame -I ../../../../01cffex_frame/src -DLINUX -DPTRADE_1_61
//g++ -o so2.so so1.cc -shared -fpic -DLINUX -DPTRADE_1_61 -I../frame -I../../../sqlapi/include -I ../../../../01cffex_frame/src ../../../../01cffex_frame/libcffexd.a -L/home/cffex/01git/02cffex_eye/sqlapi/lib -lsqlapi  ../frame/db_command.cpp ../frame/db_connection.cpp