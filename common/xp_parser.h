#ifndef XP_PARSER_H
#define XP_PARSER_H

#include <cffex/net/parser.h>
#include "xp_header.h"
#include <stdint.h>

enum {
    XP_REQUEST             = 0XA1,
    XP_RESPONSE            = 0XA2,

    XP_HEARTBEAT           = 0XA3
};

enum protocol_type
{
    PT_SBE                  = 0x00,
    PT_TLV                  = 0x01,
};

class xp_decoder : public cffex::net::decoder
{
public:
	xp_decoder();
	~xp_decoder(){};

	virtual int decode(const char *buf, int len);

    uint8_t     get_type()          const { return type_; }
    uint8_t     get_ext_head_len()  const { return buf_ == NULL ? 0 : ((xp_header *)buf_)->ext_head_len; }
    uint8_t     get_version()       const { return buf_ == NULL ? 0 : ((xp_header *)buf_)->version; }
    uint8_t     get_proto_type()    const { return buf_ == NULL ? 0 : ((xp_header *)buf_)->proto_type; }
    uint16_t    get_serviceid()     const { return buf_ == NULL ? 0 : g_ntohs(((xp_header *)buf_)->service_id); }
    uint16_t    get_messageid()     const { return buf_ == NULL ? 0 : g_ntohs(((xp_header *)buf_)->message_id); }
    uint16_t    get_body_len()      const { return buf_ == NULL ? 0 : g_ntohl(((xp_header *)buf_)->body_len); }
    uint32_t    get_resultcode()    const { return buf_ == NULL ? 0 : g_ntohl(((xp_header *)buf_)->result_code); }

    virtual const char *get_buf() const { return buf_; }
    virtual int get_len() const { return len_; }
    const char* get_body() ;

    void dump() const;
    std::string to_string(int indent = 0) const;

    /** template method */
    uint32_t get_seqno() const { return buf_ == NULL ? 0 : g_ntohl(((xp_header *)buf_)->seqno); }
    bool is_request()    const { return get_type() == XP_REQUEST;  }
    bool is_response()   const { return get_type() == XP_RESPONSE; }
    bool is_heartbeat()  const { return get_type() == XP_HEARTBEAT; }

private:
    void reset();

private:
    const char *buf_;
    int len_;
    uint8_t type_;
};


class xp_encoder : public cffex::net::encoder
{
public:
	xp_encoder() {};
    virtual	~xp_encoder() {};

    virtual void        encode();
//    virtual const char *get_buf() const { return buffer_.base(); }
//    virtual int         get_len() const { return buffer_.len();  }

    void init(uint8_t type);
    void set_ext_head_len(uint8_t len);
    void set_version(uint8_t version);
    void set_proto_type(uint8_t proto_type);

    void set_serviceid(uint16_t servicd_id);
    void set_messageid(uint16_t message_id);

    void set_seqno(uint32_t seqno);
    void set_buf(const void *buf, uint16_t len);
    void set_resultcode(uint32_t result_code);

    void set_len();

    /** template method */
    void heartbeat() { init(XP_HEARTBEAT); }

};


#endif
