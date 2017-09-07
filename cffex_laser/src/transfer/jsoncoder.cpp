#include "jsoncoder.h"

json_encoder::json_encoder(bool is_root) : is_root_(is_root)
{
    json_ = json_object_new_object();
}

json_encoder::~json_encoder()
{
    if (is_root_ && json_)
    {
        json_object_put(json_);
    }
}

void json_encoder::set_value(const std::string& key, bool value)
{
    json_object_object_add(json_, key.c_str(), json_object_new_boolean(value));
}

void json_encoder::set_value(const std::string& key, int value)
{
    json_object_object_add(json_, key.c_str(), json_object_new_int(value));
}

void json_encoder::set_value(const std::string& key, long long value)
{
    json_object_object_add(json_, key.c_str(), json_object_new_int64(value));
}

void json_encoder::set_value(const std::string& key, double value)
{
    json_object_object_add(json_, key.c_str(), json_object_new_double(value));
}

void json_encoder::set_value(const std::string& key, const std::string& value)
{
    json_object_object_add(json_, key.c_str(), json_object_new_string(value.c_str()));
}

void json_encoder::set_value(const std::string& key, json_encoder& value)
{
    json_object_object_add(json_, key.c_str(), value.json_);
}

void json_encoder::set_value(const std::string& key, const std::vector<bool>& values)
{
    json_object* array = json_object_new_array();
    for (std::vector<bool>::const_iterator iter = values.begin();
        iter != values.end(); ++iter)
    {
        json_object_array_add(array, json_object_new_boolean(*iter));
    }
    json_object_object_add(json_, key.c_str(), array);
}

void json_encoder::set_value(const std::string& key, const std::vector<int>& values)
{
    json_object* array = json_object_new_array();
    for (std::vector<int>::const_iterator iter = values.begin();
        iter != values.end(); ++iter)
    {
        json_object_array_add(array, json_object_new_int(*iter));
    }
    json_object_object_add(json_, key.c_str(), array);
}

void json_encoder::set_value(const std::string& key, const std::vector<long long>& values)
{
    json_object* array = json_object_new_array();
    for (std::vector<long long>::const_iterator iter = values.begin();
        iter != values.end(); ++iter)
    {
        json_object_array_add(array, json_object_new_int64(*iter));
    }
    json_object_object_add(json_, key.c_str(), array);
}

void json_encoder::set_value(const std::string& key, const std::vector<double>& values)
{
    json_object* array = json_object_new_array();
    for (std::vector<double>::const_iterator iter = values.begin();
        iter != values.end(); ++iter)
    {
        json_object_array_add(array, json_object_new_double(*iter));
    }
    json_object_object_add(json_, key.c_str(), array);
}

void json_encoder::set_value(const std::string& key, const std::vector<std::string>& values)
{
    json_object* array = json_object_new_array();
    for (std::vector<std::string>::const_iterator iter = values.begin();
        iter != values.end(); ++iter)
    {
        json_object_array_add(array, json_object_new_string(iter->c_str()));
    }
    json_object_object_add(json_, key.c_str(), array);
}

void json_encoder::set_value(const std::string& key, std::vector<json_encoder>& values)
{
    json_object* array = json_object_new_array();
    for (std::vector<json_encoder>::iterator iter = values.begin();
        iter != values.end(); ++iter)
    {
        json_object_array_add(array, iter->json_);
    }
    json_object_object_add(json_, key.c_str(), array);
}

const char* json_encoder::to_string()
{
    return json_object_to_json_string(json_);
}

//////////////////////////////////////////////////////////////////////////

json_decoder::json_decoder()
    : json_(NULL), is_root_(false)
{
}

json_decoder::json_decoder(const char* str)
    : is_root_(true)
{
    json_ = json_tokener_parse(str);
}

json_decoder::~json_decoder()
{
    if (is_root_ && json_)
    {
        json_object_put(json_);
    }
}

int json_decoder::get_value(const std::string& key, bool& value)
{
    json_object* obj;
    if (!json_object_object_get_ex(json_, key.c_str(), &obj))
    {
        return -1;
    }
    value = json_object_get_boolean(obj) ? true : false;
    return 0;
}

int json_decoder::get_value(const std::string& key, int& value)
{
    json_object* obj;
    if (!json_object_object_get_ex(json_, key.c_str(), &obj))
    {
        return -1;
    }
    value = json_object_get_int(obj);
    return 0;
}

int json_decoder::get_value(const std::string& key, long long& value)
{
    json_object* obj;
    if (!json_object_object_get_ex(json_, key.c_str(), &obj))
    {
        return -1;
    }
    value = json_object_get_int64(obj);
    return 0;
}

int json_decoder::get_value(const std::string& key, double& value)
{
    json_object* obj;
    if (!json_object_object_get_ex(json_, key.c_str(), &obj))
    {
        return -1;
    }
    value = json_object_get_double(obj);
    return 0;
}

int json_decoder::get_value(const std::string& key, std::string& value)
{
    json_object* obj;
    if (!json_object_object_get_ex(json_, key.c_str(), &obj))
    {
        return -1;
    }
    value = json_object_get_string(obj);
    return 0;
}

int json_decoder::get_value(const std::string& key, json_decoder& value)
{
    json_object* obj;
    if (!json_object_object_get_ex(json_, key.c_str(), &obj))
    {
        return -1;
    }
    value.json_ = obj;
    return 0;
}

int json_decoder::get_value(const std::string& key, std::vector<bool>& values)
{
    json_object* objs;
    if (!json_object_object_get_ex(json_, key.c_str(), &objs))
    {
        return -1;
    }
    for(int i=0; i < json_object_array_length(objs); i++)
    {
        json_object* obj = json_object_array_get_idx(objs, i);
        bool value = json_object_get_boolean(obj) ? true : false;
        values.push_back(value);
    }
    return 0;
}

int json_decoder::get_value(const std::string& key, std::vector<int>& values)
{
    json_object* objs;
    if (!json_object_object_get_ex(json_, key.c_str(), &objs))
    {
        return -1;
    }
    for(int i=0; i < json_object_array_length(objs); i++)
    {
        json_object* obj = json_object_array_get_idx(objs, i);
        int value = json_object_get_int(obj);
        values.push_back(value);
    }
    return 0;
}

int json_decoder::get_value(const std::string& key, std::vector<long long>& values)
{
    json_object* objs;
    if (!json_object_object_get_ex(json_, key.c_str(), &objs))
    {
        return -1;
    }
    for (int i=0; i < json_object_array_length(objs); i++)
    {
        json_object* obj = json_object_array_get_idx(objs, i);
        int64_t value = json_object_get_int64(obj);
        values.push_back(value);
    }
    return 0;
}

int json_decoder::get_value(const std::string& key, std::vector<double>& values)
{
    json_object* objs;
    if (!json_object_object_get_ex(json_, key.c_str(), &objs))
    {
        return -1;
    }
    for (int i=0; i < json_object_array_length(objs); i++)
    {
        json_object* obj = json_object_array_get_idx(objs, i);
        int64_t value = json_object_get_double(obj);
        values.push_back(value);
    }
    return 0;
}

int json_decoder::get_value(const std::string& key, std::vector<std::string>& values)
{
    json_object* objs;
    if (!json_object_object_get_ex(json_, key.c_str(), &objs))
    {
        return -1;
    }
    for (int i=0; i < json_object_array_length(objs); i++)
    {
        json_object* obj = json_object_array_get_idx(objs, i);
        const char* value = json_object_get_string(obj);
        values.push_back(value);
    }
    return 0;
}

int json_decoder::get_value(const std::string& key, std::vector<json_decoder>& values)
{
    json_object* objs;
    if (!json_object_object_get_ex(json_, key.c_str(), &objs))
    {
        return -1;
    }
    for (int i=0; i < json_object_array_length(objs); i++)
    {
        json_object* obj = json_object_array_get_idx(objs, i);
        json_decoder j;
        j.json_ = obj;
        values.push_back(j);
    }
    return 0;
}
