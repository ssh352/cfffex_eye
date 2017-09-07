#ifndef JSON_CODER_H
#define JSON_CODER_H

#include <string>
#include <vector>
#include <cstdint>
#include <json-c/json.h>

class json_encoder
{
public:
    json_encoder(bool is_root = false);
    ~json_encoder();

    void set_value(const std::string &key, bool value);
    void set_value(const std::string &key, int value);
    void set_value(const std::string &key, long long value);
    void set_value(const std::string &key, double value);
    void set_value(const std::string &key, const std::string &value);
    void set_value(const std::string &key, json_encoder &value);

    void set_value(const std::string &key, const std::vector<bool> &values);
    void set_value(const std::string &key, const std::vector<int> &values);
    void set_value(const std::string &key, const std::vector<long long> &values);
    void set_value(const std::string &key, const std::vector<double> &values);
    void set_value(const std::string &key, const std::vector<std::string> &values);
    void set_value(const std::string &key, std::vector<json_encoder> &values);

    const char* to_string();

private:
    json_object* json_;
    bool is_root_;
};

class json_decoder
{
public:
    json_decoder();
    json_decoder(const char* str);
    ~json_decoder();

    int get_value(const std::string &key, bool &value);
    int get_value(const std::string &key, int &value);
    int get_value(const std::string &key, long long &value);
    int get_value(const std::string &key, double &value);
    int get_value(const std::string &key, std::string &value);
    int get_value(const std::string &key, json_decoder &value);

    int get_value(const std::string &key, std::vector<bool> &values);
    int get_value(const std::string &key, std::vector<int> &values);
    int get_value(const std::string &key, std::vector<long long> &values);
    int get_value(const std::string &key, std::vector<double> &values);
    int get_value(const std::string &key, std::vector<std::string> &values);
    int get_value(const std::string &key, std::vector<json_decoder> &values);

private:
    json_object* json_;
    bool is_root_;
};

class json_array_wrapper
{
public:
    json_array_wrapper() : json_(NULL), iter_(0) {}
    ~json_array_wrapper() { if (json_) json_object_put(json_); }

    bool parse(const char *str) { json_ = json_tokener_parse(str); return json_ != NULL; }

    json_object *get_next()
    {
        if (iter_ >= json_object_array_length(json_)) return NULL;
        return json_object_array_get_idx(json_, iter_++);
    }

private:
    json_object *json_;
    int iter_;
};

#endif // JSON_CODER_H
