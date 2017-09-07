#include <gtest/gtest.h>

#include "xp_parser.h"
#include <cffex/log/loghelper.h>

TEST(xp_parser, encoder_decoder)
{
	XLOG(XLOG_DEBUG, "%s\n", typeid(this).name());

	const char* data = "hello xp_protocol";
	xp_encoder en;
	en.init(XP_REQUEST);
	en.set_seqno(1);
	en.set_messageid(2);
	en.set_resultcode(3);
	en.set_serviceid(4);
	en.set_version(5);
	en.set_proto_type(6);
	en.set_buf(data, strlen(data));

	xp_decoder de;
	XLOG(XLOG_DEBUG, "en = \n%s\n", binary_dump_string(en.get_buf(), en.get_len()).c_str());
	int de_len = de.decode(en.get_buf(), en.get_len());

	EXPECT_EQ(en.get_len(), de.decode(en.get_buf(), en.get_len()));
	EXPECT_EQ(XP_REQUEST, de.get_type());
	EXPECT_EQ(1, de.get_seqno());
	EXPECT_EQ(2, de.get_messageid());
	EXPECT_EQ(3, de.get_resultcode());
	EXPECT_EQ(4, de.get_serviceid());
	EXPECT_EQ(5, de.get_version());
	EXPECT_EQ(6, de.get_proto_type());

	EXPECT_EQ(strlen(data), de.get_body_len());

	EXPECT_STREQ(data, std::string((const char*)(de.get_body()), de.get_body_len()).c_str());
}