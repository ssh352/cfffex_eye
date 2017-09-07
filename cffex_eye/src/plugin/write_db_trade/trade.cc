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

namespace
{
#define SERVICE_ID_WRITETRADE 9002
class trade : public iplugin
{
public:
    virtual std::vector<int> get_service_id()
    {
        std::vector<int> v;
        v.push_back(SERVICE_ID_WRITETRADE);
        return v;
    }
	virtual int init_so(iplugin_host *host) {
		XLOG(XLOG_DEBUG, "trade::%s\n", __FUNCTION__);
        host_ = host;
        return 0;
	}
	virtual void regist_subscribe_topic(std::vector<uint16_t> &sub_topics){
		XLOG(XLOG_DEBUG, "trade::%s \n", __FUNCTION__);
        sub_topics.push_back(cffex::ptrade::XTP_FID_Trade);
	}

	virtual int assign_task(uint16_t fid, char const *data) {
		XLOG(XLOG_DEBUG, "trade::%s, fid[0X%04X]\n", __FUNCTION__, fid);
		switch(fid)
		{
			case cffex::ptrade::XTP_FID_Trade:
				cffex::ptrade::CTradeField trade;
				trade.init_from_stream(data);

				char szsql[1024] = {0};
				sprintf(szsql, "insert into t_trade(TradingDay, SettlementGroupID, SettlementID, TradeID, Direction, OrderSysID, ParticipantID, ClientID, TradingRole, AccountID,"
					"InstrumentID, OffsetFlag, HedgeFlag, Price, Volume, TradeTime, TradeType, PriceSource, UserID, OrderLocalID, ClearingPartID, BusinessUnit) "
					"values(\'%s\', \'%s\', %d, \'%s\', %d, \'%s\', \'%s\', \'%s\', %d, \'%s\', \'%s\', %d, %d, %f, %d, \'%s\', %d, %d, \'%s\', \'%s\', \'%s\', \'%s\')",
                    trade.TradingDay.get_value(), trade.SettlementGroupID.get_value(), trade.SettlementID.get_value(), trade.TradeID.get_value(),
                    trade.Direction.get_value(), trade.OrderSysID.get_value(), trade.ParticipantID.get_value(), trade.ClientID.get_value(),
                    trade.TradingRole.get_value(), trade.AccountID.get_value(), trade.InstrumentID.get_value(), trade.OffsetFlag.get_value(),
                    trade.HedgeFlag.get_value(), trade.Price.get_value(), trade.Volume.get_value(), trade.TradeTime.get_value(),
                    trade.TradeType.get_value(), trade.PriceSource.get_value(), trade.UserID.get_value(), trade.OrderLocalID.get_value(),
                    trade.ClearingPartID.get_value(), trade.BusinessUnit.get_value());

                sqlstring_command *cmd = new sqlstring_command(DB_NAME_MYSQL, szsql);
                host_->post_sql_command(cmd, true);
				break;
		}
        return 0;
	}

	virtual int assign_request(request_info *req_info){return 0;}

private:
    iplugin_host *host_;
};

} // namespace

#ifdef __cplusplus
extern "C"
{
#endif

void* create(){
    trade *p = new trade();
	return (void*)p;
}

void destroy(void* p){
	delete (trade*)p;
}

#ifdef __cplusplus
}
#endif
