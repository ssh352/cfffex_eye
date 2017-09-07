#include "xp_parser.h"
#include <cffex/log/loghelper.h>

const int XP_HEADER_LEN = sizeof(xp_header);

xp_decoder::xp_decoder()
{
	reset();
}

void xp_decoder::reset()
{
	buf_ = NULL;
	len_ = 0;
	type_ = 0;
}

int xp_decoder::decode(const char *buf, int len)
{
	reset();
	buf_ = buf;
	len_ = len;

    if (buf == NULL || len <= 0) {
        XLOG(XLOG_WARNING,"xp_decoder::%s, invalidate buffer[0X%0X], len[%d]\n", __FUNCTION__, buf, len);
        return -1;
    }
    if (len < XP_HEADER_LEN) {
        return 0;
    }

	xp_header *h = (xp_header*)buf_;
	int package_len = XP_HEADER_LEN + h->ext_head_len + g_ntohl(h->body_len);
	type_ = h->type;

	if(len < package_len)
	{
		return 0;
	}

	return package_len;
}

const char* xp_decoder::get_body()
{
	xp_header *h = (xp_header*)buf_;
	return buf_ + XP_HEADER_LEN + h->ext_head_len;
}

void xp_decoder::dump() const {
	XLOG(XLOG_DEBUG, "xp_decoder::%s, %s\n", __FUNCTION__, to_string().c_str());
}

std::string xp_decoder::to_string(int indent) const {
	std::string strindent(indent * 2, ' ');
	xp_header *h = (xp_header*)buf_;

	char sz[256] = {0};
	int len = snprintf(sz, sizeof(sz)-1, "%stype[0x%04X], ext_head_len[%d], version[%d], proto_type[%d], service_id[%d], message_id[%d], seqno[%d], body_len[%d], result_code[%d]",
		strindent.c_str(), h->type, h->ext_head_len, h->version, h->proto_type, g_ntohs(h->service_id), g_ntohs(h->message_id), g_ntohl(h->seqno), g_ntohl(h->body_len), g_ntohl(h->result_code));
	sz[len] = '\n';
	return sz;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/


void xp_encoder::init(uint8_t type)
{
	buffer_.reset_loc(XP_HEADER_LEN);
	xp_header *h = (xp_header*)(buffer_.base());
	h->type = type;
	h->ext_head_len = 0;
	h->version = 0;
	h->proto_type = PT_SBE;
	h->body_len = 0;
	h->result_code = 0;
	h->message_id = 0;
	h->service_id = 0;
	h->seqno = 0;
}
void xp_encoder::set_ext_head_len(uint8_t len)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->ext_head_len = len;
}
void xp_encoder::set_version(uint8_t version)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->version = version;
}
void xp_encoder::set_proto_type(uint8_t proto_type)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->proto_type = proto_type;
}

void xp_encoder::set_serviceid(uint16_t service_id)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->service_id = g_htons(service_id);
}
void xp_encoder::set_messageid(uint16_t message_id)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->message_id = g_htons(message_id);
}
void xp_encoder::set_seqno(uint32_t seqno)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->seqno = g_htonl(seqno);
}

void xp_encoder::set_resultcode(uint32_t result_code)
{
	xp_header *h = (xp_header*)(buffer_.base());
	h->result_code = g_htonl(result_code);
}

void xp_encoder::set_buf(const void *buf, uint16_t len)
{
    buffer_.append((const char *)buf, len);
    xp_header *h   = (xp_header *)(buffer_.base());
    h->body_len    = g_htonl(len);
}

void xp_encoder::set_len()
{
    xp_header *h   = (xp_header *)(buffer_.base());
    h->body_len    = g_htonl(buffer_.len() - sizeof(xp_header) - h->ext_head_len);
}

void xp_encoder::encode() {
}

