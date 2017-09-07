#include <gtest/gtest.h>
#include <cffex/log/loghelper.h>
#include <cffex/http/http_server.h>
#include <cffex/net/requester.h>
#include <string>
#include "xp_parser.h"
#include "sbe_test.h"
#include "proto_transfer.h"
#include "jsoncoder.h"
#include "GetCar.h"
#include "tlv.h"

TEST(xp_http_transfer, http_to_tlv)
{
    std::map<std::string, std::string> map_pram;
    map_pram["params"] = "[100, \"param2\"]";

    xp_encoder xpen;
    proto_transfer *transfer = proto_transfer_factory::get_instance()->get_transfer("tlvTest");
    xpen.init(XP_REQUEST);
    if (transfer->http_to_xp("tlvTest", "reqTest", map_pram, &xpen) != 0)
    {
        XLOG(XLOG_DEBUG, "http_to_tlv failed %s\n", typeid(this).name());
        return;
    }

    tlv_decoder tlvde(xpen.get_buf() + sizeof(xp_header), xpen.get_len() - sizeof(xp_header));
    int32_t v1;
    tlvde.get_value(1, v1);
    EXPECT_EQ(100, v1);
    std::string v2;
    tlvde.get_value(2, v2);
    EXPECT_STREQ("param2", v2.c_str());
}

TEST(xp_http_transfer, tlv_to_http)
{
    xp_encoder en;
    en.init(XP_RESPONSE);
    en.set_serviceid(100);
    en.set_messageid(2);
    en.set_proto_type(PT_TLV);

    // tlv_schema/test_service.xml
    tlv_encoder tlven(*en.get_buffer());
    tlven.set_value(1, 100);
    tlven.set_value(2, "param2");
    tlv_encoder mytype = tlven.add_tlv(3);
    mytype.set_value(1, 101);
    mytype.set_value(2, "mytest.param2");
    mytype.set_length(); // end mytype
    tlven.set_value(4, "param4.1");
    tlven.set_value(4, "param4.2");

    en.set_len();

    xp_decoder de;
    de.decode(en.get_buf(), en.get_len());

    proto_transfer *t = proto_transfer_factory::get_instance()->get_transfer(de.get_proto_type());
    cffex::http::http_encoder hen;
    hen.init(200);
    hen.head("Content-Type", "text/html");
    EXPECT_EQ(0, t->xp_to_http(&de, &hen));

    std::string httprsp(hen.get_buf(), hen.get_len());
    XLOG(XLOG_DEBUG, "httprsp =\n%s\n", httprsp.c_str());

    cffex::http::http_decoder hde;
    hde.decode(hen.get_buf(), hen.get_len());

    json_decoder json(hde.body().c_str());

    int value = -1;
    json.get_value("param1", value);
    EXPECT_EQ(100, value);

    std::string svalue;
    json.get_value("param2", svalue);
    EXPECT_STREQ("param2", svalue.c_str());
}

TEST(xp_http_transfer, http_to_sbe)
{
    std::map<std::string, std::string> map_pram;
    map_pram["params"] = "[1, 2, null, \"string2\"]";

    xp_encoder xpen;
    proto_transfer *transfer = proto_transfer_factory::get_instance()->get_transfer(PT_SBE);
    xpen.init(XP_REQUEST);
    if (transfer->http_to_xp("example", "getCar", map_pram, &xpen) != 0)
    {
        XLOG(XLOG_DEBUG, "http_to_xp failed %s\n", typeid(this).name());
        return;
    }

    example::MessageHeader hdr((char *)xpen.get_buf() + sizeof(xp_header), xpen.get_len() - sizeof(xp_header), 0);
    EXPECT_EQ(GetCar::sbeBlockLength(), hdr.blockLength());
    EXPECT_EQ(0, hdr.version());

    example::GetCar getcar((char *)xpen.get_buf() + sizeof(xp_header) + MessageHeader::encodedLength(),
        xpen.get_len() - sizeof(xp_header) - MessageHeader::encodedLength());

    EXPECT_EQ(1, getcar.param1());
    EXPECT_EQ(2, getcar.param2());
    EXPECT_STREQ("", getcar.getParam3AsString().c_str());
    EXPECT_STREQ("string2", getcar.getParam4AsString().c_str());
}

TEST(xp_http_transfer, sbe_to_http)
{
    char buffer[2048] = { 0 };
    example::MessageHeader hdr;
    example::Car car;
    std::size_t encodeHdrLength = encodeHdrCar(hdr, car, buffer, 0, sizeof(buffer));
    std::size_t encodeMsgLength = encodeCar(car, buffer, hdr.encodedLength(), sizeof(buffer));

    xp_encoder en;
    en.init(XP_RESPONSE);
    en.set_serviceid(1);
    en.set_messageid(1);
    en.set_proto_type(PT_SBE);
    en.set_buf(buffer, encodeHdrLength + encodeMsgLength);

    xp_decoder de;
    de.decode(en.get_buf(), en.get_len());

    proto_transfer *t = proto_transfer_factory::get_instance()->get_transfer(de.get_proto_type());
    cffex::http::http_encoder hen;
    hen.init(200);
    hen.head("Content-Type", "text/html");
    t->xp_to_http(&de, &hen);

    std::string httprsp(hen.get_buf(), hen.get_len());
    XLOG(XLOG_DEBUG, "httprsp =\n%s\n", httprsp.c_str());

    cffex::http::http_decoder hde;
    hde.decode(hen.get_buf(), hen.get_len());

    json_decoder json(hde.body().c_str());

    int value = -1;
    json.get_value("version", value);
    EXPECT_EQ(0, value);

    value = -1;
    json.get_value("errorCode", value);
    EXPECT_EQ(0, value);

    value = -1;
    json.get_value("serialNumber", value);
    EXPECT_EQ(1234, value);

    std::string svalue;
    json.get_value("manufacturer", svalue);
    EXPECT_STREQ("Honda", svalue.c_str());

    svalue.clear();
    json.get_value("activationCode", svalue);
    EXPECT_STREQ("deadbeef", svalue.c_str());
}
