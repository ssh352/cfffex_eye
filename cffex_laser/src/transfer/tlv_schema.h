#ifndef TLV_SCHEMA_H
#define TLV_SCHEMA_H

#include <string>
#include <vector>
#include <map>
#include <cffex/singleton.h>

enum TLV_DATA_TYPE
{
    TLV_TYPE_USERDEF = 0,
    TLV_TYPE_UINT32  = 1,
    TLV_TYPE_INT32,
    TLV_TYPE_UINT64,
    TLV_TYPE_INT64,
    TLV_TYPE_FLOAT,
    TLV_TYPE_DOUBLE,
    TLV_TYPE_STRING,
};

struct tlv_field
{
    std::string name;
    int         id;
    int         type;
    std::string type_name;
    bool        is_array;
};

struct tlv_message
{
    std::string name;
    int         id;
    std::vector<tlv_field> fields;
};

struct tlv_schema
{
    std::string name;
    int         id;
    std::map<int, tlv_message *> msgs;
    std::map<std::string, tlv_message *> msgs_by_name;

    tlv_schema() {}
    ~tlv_schema()
    {
        for (auto itr = msgs_by_name.begin(); itr != msgs_by_name.end(); ++itr)
        {
            delete itr->second;
        }
    }

private:
    tlv_schema(const tlv_schema &);
    const tlv_schema &operator=(const tlv_schema &);
};

class tlv_schema_manager : public cffex::singleton<tlv_schema_manager>
{
public:
    bool load(const std::string &dir_name);

    const tlv_schema *get_schema(int id);
    const tlv_schema *get_schema(const std::string &name);

    ~tlv_schema_manager()
    {
        for (auto itr = schemas_.begin(); itr != schemas_.end(); ++itr)
        {
            delete itr->second;
        }
    }

private:
    bool load_schema(const std::string &file_name);

private:
    std::map<int, tlv_schema *> schemas_;
    std::map<std::string, tlv_schema *> schemas_by_name_;
};

#endif // TLV_SCHEMA_H
