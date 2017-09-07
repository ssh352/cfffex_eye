#ifndef TLV_H
#define TLV_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <cffex/buffer.h>

class tlv_encoder
{
public:
    tlv_encoder(cffex::buffer &buff);
    ~tlv_encoder();

    void set_value(uint16_t id, uint32_t v);
    void set_value(uint16_t id, int32_t v);
    void set_value(uint16_t id, uint64_t v);
    void set_value(uint16_t id, int64_t v);
    void set_value(uint16_t id, float v);
    void set_value(uint16_t id, double v);
    void set_value(uint16_t id, const char *ptr, size_t length);
    void set_value(uint16_t id, const char *str);
    void set_value(uint16_t id, const std::string &str);

    tlv_encoder add_tlv(uint16_t id);
    void set_length(); // call by destructor

private:
    cffex::buffer &buff_;
    char *ptr_;
    size_t len_;
};

class tlv_decoder
{
public:
    tlv_decoder();
    tlv_decoder(const char *ptr, size_t length);

    void decode(const char *ptr, size_t length);

    bool get_value(uint16_t id, uint32_t &v);
    bool get_value(uint16_t id, int32_t &v);
    bool get_value(uint16_t id, uint64_t &v);
    bool get_value(uint16_t id, int64_t &v);
    bool get_value(uint16_t id, float &v);
    bool get_value(uint16_t id, double &v);
    bool get_value(uint16_t id, std::string &str);
    bool get_value(uint16_t id, tlv_decoder &tlv);
    bool get_value(uint16_t id, char **value, size_t *length);

    bool get_value(uint16_t id, std::vector<uint32_t> &v);
    bool get_value(uint16_t id, std::vector<int32_t> &v);
    bool get_value(uint16_t id, std::vector<uint64_t> &v);
    bool get_value(uint16_t id, std::vector<int64_t> &v);
    bool get_value(uint16_t id, std::vector<float> &v);
    bool get_value(uint16_t id, std::vector<double> &v);
    bool get_value(uint16_t id, std::vector<std::string> &v);
    bool get_value(uint16_t id, std::vector<tlv_decoder> &v);

private:
    std::multimap<uint16_t, const char *> fields_;
};

#endif // TLV_H
