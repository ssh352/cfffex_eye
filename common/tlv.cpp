#include "tlv.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <cffex/platform.h>

#define TLV_VALUE_ALIGN(size) (((size) + 3) & ~3)

struct tlv_field
{
    uint16_t id;
    uint16_t length;
    char value[0];
};

tlv_encoder::tlv_encoder(cffex::buffer &buff)
    : buff_(buff)
    , ptr_(NULL)
    , len_(0)
{
}

tlv_encoder::~tlv_encoder()
{
    if (ptr_ != NULL)
    {
        set_length();
    }
}

void tlv_encoder::set_value(uint16_t id, uint32_t v)
{
    buff_.reserve(sizeof(tlv_field) + sizeof(v));

    tlv_field *field = (tlv_field *)buff_.top();
    field->id = g_htons(id);
    field->length = g_htons(sizeof(v));
    *((uint32_t *)field->value) = g_htonl(v);

    buff_.inc_loc(sizeof(tlv_field) + sizeof(v));

    if (ptr_ != NULL)
    {
        len_ += sizeof(tlv_field) + sizeof(v);
    }
}

void tlv_encoder::set_value(uint16_t id, int32_t v)
{
    set_value(id, (uint32_t)v);
}

void tlv_encoder::set_value(uint16_t id, uint64_t v)
{
    buff_.reserve(sizeof(tlv_field) + sizeof(v));

    tlv_field *field = (tlv_field *)buff_.top();
    field->id = g_htons(id);
    field->length = g_htons(sizeof(v));
    *((uint64_t *)field->value) = g_htonll(v);

    buff_.inc_loc(sizeof(tlv_field) + sizeof(v));

    if (ptr_ != NULL)
    {
        len_ += sizeof(tlv_field) + sizeof(v);
    }
}

void tlv_encoder::set_value(uint16_t id, int64_t v)
{
    set_value(id, (uint64_t)v);
}

void tlv_encoder::set_value(uint16_t id, float v)
{
    uint32_t tmp = *(uint32_t *)(&v);
    set_value(id, tmp);
}

void tlv_encoder::set_value(uint16_t id, double v)
{
    uint64_t tmp = *(uint64_t *)(&v);
    set_value(id, tmp);
}

void tlv_encoder::set_value(uint16_t id, const char *ptr, size_t length)
{
    
    size_t fact_len = TLV_VALUE_ALIGN(length);
    buff_.reserve(sizeof(tlv_field) + fact_len);

    tlv_field *field = (tlv_field *)buff_.top();
    field->id = g_htons(id);
    field->length = g_htons(length);
    ::memcpy(field->value, ptr, length);
    if (fact_len > length)
    {
        ::memset(field->value + length, 0, fact_len - length);
    }

    buff_.inc_loc(sizeof(tlv_field) + fact_len);

    if (ptr_ != NULL)
    {
        len_ += sizeof(tlv_field) + fact_len;
    }
}

void tlv_encoder::set_value(uint16_t id, const char *str)
{
    set_value(id, str, ::strlen(str));
}

void tlv_encoder::set_value(uint16_t id, const std::string &str)
{
    set_value(id, str.c_str(), str.length());
}

tlv_encoder tlv_encoder::add_tlv(uint16_t id)
{
    buff_.reserve(sizeof(tlv_field));
    tlv_encoder sub(buff_);
    sub.ptr_ = (char *)buff_.top();
    tlv_field *field = (tlv_field *)sub.ptr_;
    field->id = g_htons(id);
    buff_.inc_loc(sizeof(tlv_field));
    len_ = 0;
    return sub;
}

void tlv_encoder::set_length()
{
    tlv_field *field = (tlv_field *)ptr_;
    field->length = g_htons(len_);
    ptr_ = NULL;
}

//////////////////////////////////////////////////////////////////////////

tlv_decoder::tlv_decoder()
{
}

tlv_decoder::tlv_decoder(const char *ptr, size_t length)
{
    decode(ptr, length);
}

void tlv_decoder::decode(const char *ptr, size_t length)
{
    const char *p = ptr;
    while (p + sizeof(tlv_field) <= ptr + length)
    {
        tlv_field *field = (tlv_field *)p;
        size_t len = g_ntohs(field->length);
        if (p + sizeof(tlv_field) + len > ptr + length)
        {
            break;
        }

        fields_.insert(std::make_pair(g_ntohs(field->id), p));
        size_t fact_len = TLV_VALUE_ALIGN(len);
        p += fact_len + sizeof(tlv_field);
    }
}

bool tlv_decoder::get_value(uint16_t id, char **value, size_t *length)
{
    std::multimap<uint16_t, const char *>::iterator itr = fields_.find(id);
    if (itr == fields_.end())
    {
        return false;
    }
    tlv_field *field = (tlv_field *)itr->second;
    *value = field->value;
    *length = g_ntohs(field->length);
    return true;
}

bool tlv_decoder::get_value(uint16_t id, uint32_t &v)
{
    char *value;
    size_t length;
    if (!get_value(id, &value, &length) || length != sizeof(uint32_t))
    {
        return false;
    }
    v = g_ntohl(*(uint32_t *)value);
    return true;
}

bool tlv_decoder::get_value(uint16_t id, int32_t &v)
{
    return get_value(id, (uint32_t &)(v));
}

bool tlv_decoder::get_value(uint16_t id, uint64_t &v)
{
    char *value;
    size_t length;
    if (!get_value(id, &value, &length) || length != sizeof(uint64_t))
    {
        return false;
    }
    v = g_ntohll(*(uint64_t *)value);
    return true;
}

bool tlv_decoder::get_value(uint16_t id, int64_t &v)
{
    return get_value(id, (uint64_t &)v);
}

bool tlv_decoder::get_value(uint16_t id, float &v)
{
    return get_value(id, (uint32_t &)v);
}

bool tlv_decoder::get_value(uint16_t id, double &v)
{
    return get_value(id, (uint64_t &)v);
}

bool tlv_decoder::get_value(uint16_t id, std::string &str)
{
    char *value;
    size_t length;
    if (!get_value(id, &value, &length))
    {
        return false;
    }
    str = std::string(value, length);
    return true;
}

bool tlv_decoder::get_value(uint16_t id, tlv_decoder &tlv)
{
    char *value;
    size_t length;
    if (!get_value(id, &value, &length))
    {
        return false;
    }
    tlv.decode(value, length);
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<uint32_t> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        if (field->length != sizeof(uint32_t))
        {
            return false;
        }
        v.push_back(g_ntohl(*(uint32_t *)field->value)) ;
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<int32_t> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        if (field->length != sizeof(int32_t))
        {
            return false;
        }
        v.push_back(g_ntohl(*(int32_t *)field->value));
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<uint64_t> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        if (field->length != sizeof(uint64_t))
        {
            return false;
        }
        v.push_back(g_ntohll(*(uint64_t *)field->value));
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<int64_t> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        if (field->length != sizeof(int64_t))
        {
            return false;
        }
        v.push_back(g_ntohll(*(int64_t *)field->value));
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<float> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        if (field->length != sizeof(float))
        {
            return false;
        }
        uint32_t value = g_ntohl(*(uint32_t *)field->value);
        v.push_back(*(float *)&value);
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<double> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        if (field->length != sizeof(double))
        {
            return false;
        }
        uint64_t value = g_ntohll(*(uint64_t *)field->value);
        v.push_back(*(double *)&value);
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<std::string> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        v.push_back(std::string(field->value, g_ntohs(field->length)));
    }
    return true;
}

bool tlv_decoder::get_value(uint16_t id, std::vector<tlv_decoder> &v)
{
    std::pair<std::multimap<uint16_t, const char *>::iterator,
        std::multimap<uint16_t, const char *>::iterator> itr_pair = fields_.equal_range(id);
    for (std::multimap<uint16_t, const char *>::iterator itr = itr_pair.first; itr != itr_pair.second; ++itr)
    {
        tlv_field *field = (tlv_field *)itr->second;
        tlv_decoder tlv(field->value, g_ntohs(field->length));
        v.push_back(tlv);
    }
    return true;
}
