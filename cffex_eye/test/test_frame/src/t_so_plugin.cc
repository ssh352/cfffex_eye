#include <gtest/gtest.h>

#include "so_plugin.hpp"
#include "threadtpl.hpp"
#include <cffex/ptrade/field/xtp_fieldid.h>
#include <cffex/ptrade/field/xtp_field.h>
#include <cffex/ptrade/parser/xmp_parser.h>
#include <cffex/ptrade/parser/xtp_parser.h>
#include <cffex/flow/cache_flow.h>
#include <cffex/flow/file_flow.h>
#include "plugin.h"



TEST(so_plugin, service_id)
{
	so_plugin so;
	pulgin p;
	so.set_plugin(&p);

	std::vector<int> v;
	v = so.get_service_id();
	EXPECT_EQ(8001, v[0]);
	EXPECT_EQ(8002, v[1]);

}

TEST(so_plugin, so_assign_task)
{
	threadtpl<so_plugin> *thrd = new threadtpl<so_plugin>();

	so_plugin* sop = new so_plugin();
	pulgin p;
	cffex::file::remove("./t_file_flow.con");
	cffex::file_flow flow(".", "t_file_flow");

	sop->init_default_constructor(&p, &flow, thrd);

	cffex::ptrade::xtp_encoder encoder;
	encoder.init(cffex::ptrade::XTP_REQUEST, cffex::ptrade::TID_MemAlter);
	cffex::ptrade::CTradeField trade;
	trade.TradingDay = "20170814";
	trade.SettlementGroupID = "SG01";
	trade.TradeID = "000001";
	trade.Direction = '1';
	trade.InstrumentID = "IF1709";
	trade.Price = 3421.56;
	trade.Volume = 20;
	trade.TradeTime = "14:15:23";
	encoder.set_field(trade);
	encoder.encode();
	trade.dump();

//	XLOG(XLOG_DEBUG, "en = \n%s\n", binary_dump_string(encoder.get_buf(), encoder.get_len()).c_str());
	flow.append(encoder.get_buf(), encoder.get_len());

	thrd->push_plugin(sop);
	thrd->start();

	sleep(3);
	thrd->stop();
	delete sop;
}