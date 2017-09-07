#ifndef SBE_SCHEMA_H
#define SBE_SCHEMA_H

#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <cffex/singleton.h>

//////////////////////////////////////////////////////////////////////////
// types

enum SBE_TYPE
{
    SBE_USER = 0,
    SBE_CHAR = 1,
    SBE_INT8,
    SBE_UINT8,
    SBE_INT16,
    SBE_UINT16,
    SBE_INT32,
    SBE_UINT32,
    SBE_INT64,
    SBE_UINT64,
    SBE_FLOAT,
    SBE_DOUBLE,
};

enum SBE_TYPE_TYPE
{
    SBE_TYPE_TYPE = 1,
    SBE_TYPE_COMPOS,
    SBE_TYPE_ENUM,
    SBE_TYPE_SET,
};

struct sbe_type_base
{
    int sbe_type_type;
    std::string name;
    int byte_use;
    int offset; // 组合类型中的偏移
    bool inited;

    sbe_type_base() : byte_use(0), offset(0), inited(false) {}
    virtual ~sbe_type_base() {}
};

enum SBE_PRESENCE
{
    SBE_PRESENCE_NO = 0,
    SBE_PRESENCE_OPTION = 1,
    SBE_PRESENCE_CONSTANT = 2,
};

struct sbe_type : public sbe_type_base
{
    int primitive_type;
    std::string user_type;
    int length;
    std::string min_value;
    std::string max_value;
    int char_encodeing;
    int since_version;
    int presence;
    std::string constant_value;
    bool inner_type;

    sbe_type() : primitive_type(0), length(1), char_encodeing(0), since_version(0), presence(SBE_PRESENCE_NO), inner_type(false) {}
};

struct sbe_enum : public sbe_type_base
{
    int encoding_type;
    //std::map<std::string, int> values;
    std::map<int, std::string> values_revs;
};

struct sbe_set : public sbe_type_base
{
    int encoding_type;
    std::map<std::string, int> choices;
    //std::map<std::string, std::string> choices_revs;
};

struct sbe_composite : public sbe_type_base
{
    std::vector<sbe_type *> types;

    std::map<std::string, sbe_type_base *> inner_types; // enum, set

    sbe_composite() {}
    virtual ~sbe_composite()
    {
        for (std::vector<sbe_type *>::iterator itr = types.begin(); itr != types.end(); ++itr)
        {
            delete *itr;
        }
        for (std::map<std::string, sbe_type_base *>::iterator itr = inner_types.begin();
        itr != inner_types.end(); ++itr)
        {
            delete itr->second;
        }
    }

private:
    sbe_composite(const sbe_composite &);
    const sbe_composite &operator=(const sbe_composite &);
};

//////////////////////////////////////////////////////////////////////////
// message

struct sbe_msg_field
{
    std::string name;
    int type;
    std::string type_name;
    int since_version;
    int presence;
    std::string value_ref;

    int byte_use;
    int offset;
};

struct sbe_msg_data
{
    std::string name;
    std::string type;
    int since_version;
};

struct sbe_msg_group
{
    std::string name;
    std::string dimension_type;
    std::vector<sbe_msg_field> fileds;
    int block_len;
    std::vector<sbe_msg_group> groups;
    std::vector<sbe_msg_data> datas;
};

struct sbe_msg
{
    std::string name;
    int id;
    std::vector<sbe_msg_field> fileds;
    int block_len;
    std::vector<sbe_msg_group> groups;
    std::vector<sbe_msg_data> datas;
};

//////////////////////////////////////////////////////////////////////////
//

enum SBE_BYTE_ORDER
{
    SBE_BYTE_ORDER_LITTLE = 0,
    SBE_BYTE_ORDER_GIG = 1,
};

struct sbe_schema
{
    int id;
    std::string name;
    int version;
    int byte_order;

    std::map<std::string, sbe_type_base *> types; // type, enum, set, composite

    std::map<int, sbe_msg *> msgs;
    std::map<std::string, sbe_msg *> msgs_by_name;

    sbe_schema() : byte_order(SBE_BYTE_ORDER_LITTLE) {}
    virtual ~sbe_schema()
    {
        for (std::map<std::string, sbe_type_base *>::iterator itr = types.begin();
            itr != types.end(); ++itr)
        {
            delete itr->second;
        }
        for (std::map<int, sbe_msg *>::iterator itr = msgs.begin();
            itr != msgs.end(); ++itr)
        {
            delete itr->second;
        }
    }
private:
    sbe_schema(const sbe_schema &);
    const sbe_schema &operator=(const sbe_schema &);
};

class sbe_schema_manager : public cffex::singleton<sbe_schema_manager>
{
public:
    bool load(const std::string &dir_name);
    const sbe_schema *get_schema(int id);
    const sbe_schema *get_schema(const std::string &name);

    ~sbe_schema_manager()
    {
        for (std::map<int, sbe_schema *>::iterator itr = schemas_.begin();
            itr != schemas_.end(); ++itr)
        {
            delete itr->second;
        }
    }

private:
    void load_schema(const std::string &file_name);

private:
    std::map<int, sbe_schema *> schemas_;
    std::map<std::string, sbe_schema *> schemas_by_name_;
};

#endif // SBE_SCHEMA_H
