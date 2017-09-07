#include "tlv_schema.h"
#include <cffex/common/dir_reader.h>
#include <cffex/tinyxml2/tinyxml2.h>
#include <cffex/log/loghelper.h>

using namespace cffex::common;
using namespace cffex::tinyxml2;

bool tlv_schema_manager::load(const std::string &dir_name)
{
    dir_reader dr("*.xml");
    char filename[2048] = { 0 };
    if (!dr.open(dir_name.c_str()))
    {
        XLOG(XLOG_ERROR, "tlv_schema_manager::%s open dir failed[%s]\n", __FUNCTION__, dir_name.c_str());
        return false;
    }
    if (!dr.get_first_file_name(filename, sizeof(filename)))
    {
        XLOG(XLOG_WARNING, "tlv_schema_manager::%s get_first_file_name failed[%s]\n", __FUNCTION__, dir_name.c_str());
        return false;
    }
    if (!load_schema(filename))
        return false;

    while (dr.get_next_file_name(filename, sizeof(filename)))
    {
        if (!load_schema(filename))
            return false;
    }
    return true;
}

const tlv_schema *tlv_schema_manager::get_schema(int id)
{
    std::map<int, tlv_schema *>::iterator itr = schemas_.find(id);
    return itr != schemas_.end() ? itr->second : NULL;
}

const tlv_schema *tlv_schema_manager::get_schema(const std::string &name)
{
    std::map<std::string, tlv_schema *>::iterator itr = schemas_by_name_.find(name);
    return itr != schemas_by_name_.end() ? itr->second : NULL;
}

namespace
{
    static class tlv_type_map
    {
    public:
        tlv_type_map()
        {
            map_["int32"]  = TLV_TYPE_INT32;
            map_["uint32"] = TLV_TYPE_UINT32;
            map_["int64"]  = TLV_TYPE_INT64;
            map_["uint64"] = TLV_TYPE_UINT64;
            map_["float"]  = TLV_TYPE_FLOAT;
            map_["double"] = TLV_TYPE_DOUBLE;
            map_["string"] = TLV_TYPE_STRING;
        }

        int get(const std::string &str)
        {
            std::map<std::string, int>::iterator itr = map_.find(str);
            return itr != map_.end() ? itr->second : TLV_TYPE_USERDEF;
        }

    private:
        std::map<std::string, int> map_;
    } tlv_type_map;
}

bool tlv_schema_manager::load_schema(const std::string &file_name)
{
    XMLDocument xmldoc;
    if (xmldoc.LoadFile(file_name.c_str()) != XML_SUCCESS)
    {
        XLOG(XLOG_ERROR, "tlv_schema_manager::%s load xml failed[%s]\n", __FUNCTION__, file_name.c_str());
        return false;
    }
    XMLElement *xmlRoot = xmldoc.RootElement();
    if (xmlRoot == NULL)
    {
        XLOG(XLOG_ERROR, "tlv_schema_manager::%s get xml root failed[%s]\n", __FUNCTION__, file_name.c_str());
        return false;
    }

    tlv_schema *schema = new tlv_schema;
    schema->id = xmlRoot->IntAttribute("id");
    const char *pname;
    if ((pname = xmlRoot->Attribute("name")) == NULL)
    {
        XLOG(XLOG_ERROR, "tlv_schema_manager::%s schema no attribute.name[%s]\n", __FUNCTION__, file_name.c_str());
        return false;
    }
    schema->name = pname;

    for (XMLElement * xmlMessage = xmlRoot->FirstChildElement("message");
        xmlMessage != NULL; xmlMessage = xmlMessage->NextSiblingElement("message"))
    {

        tlv_message *msg = new tlv_message;
        const char *ptr;
        if ((ptr = xmlMessage->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "tlv_schema_manager::%s message no attribute.name, msg[%s], file[%s]\n", __FUNCTION__,
                schema->name.c_str(), file_name.c_str());
            return false;
        }
        msg->name = ptr;
        msg->id = xmlMessage->IntAttribute("id");

        for (XMLElement * xmlField = xmlMessage->FirstChildElement("field");
            xmlField != NULL; xmlField = xmlField->NextSiblingElement())
        {
            tlv_field field;
            if ((ptr = xmlField->Attribute("name")) == NULL)
            {
                XLOG(XLOG_ERROR, "tlv_schema_manager::%s field no attribute.name, msg[%s], file[%s]\n", __FUNCTION__,
                    msg->name.c_str(), file_name.c_str());
                return false;
            }
            field.name = ptr;
            field.id = xmlField->IntAttribute("id");
            field.is_array = false;
            if ((ptr = xmlField->Attribute("array")) != NULL)
            {
                if (std::string("true") == ptr)
                {
                    field.is_array = true;
                }
            }

            if ((ptr = xmlField->Attribute("type")) == NULL)
            {
                XLOG(XLOG_ERROR, "tlv_schema_manager::%s field no attribute.type, msg[%s], file[%s]\n", __FUNCTION__,
                    msg->name.c_str(), file_name.c_str());
                return false;
            }
            field.type_name = ptr;
            field.type = tlv_type_map.get(field.type_name);

            msg->fields.push_back(field);
        }

        if (schema->msgs_by_name.find(msg->name) != schema->msgs_by_name.end() ||
            (msg->id != 0 && schema->msgs.find(msg->id) != schema->msgs.end()))
        {
            XLOG(XLOG_WARNING, "tlv_schema_manager::%s message id or name may be duplicated?, msg[%s], file[%s]\n",
                 __FUNCTION__,  msg->name.c_str(), file_name.c_str());
            return false;
        }

        schema->msgs_by_name[msg->name] = msg;
        if (msg->id != 0)
            schema->msgs[msg->id] = msg;
    }

    // check all type is defined
    for (auto itr = schema->msgs_by_name.begin(); itr != schema->msgs_by_name.end(); ++itr)
    {
        tlv_message *msg = itr->second;
        for (auto itrField = msg->fields.begin(); itrField != msg->fields.end(); ++itrField)
        {
            if (itrField->type == TLV_TYPE_USERDEF && schema->msgs_by_name.find(itrField->type_name) == schema->msgs_by_name.end())
            {
                XLOG(XLOG_ERROR, "tlv_schema_manager::%s type[%s] not found, msg[%s], file[%s]\n",
                    __FUNCTION__,  itrField->type_name.c_str(), msg->name.c_str(), file_name.c_str());
                return false;
            }
        }
    }

    if (schemas_.find(schema->id) != schemas_.end() || schemas_by_name_.find(schema->name) != schemas_by_name_.end())
    {
        XLOG(XLOG_WARNING, "tlv_schema_manager::%s schema id or name may be duplicated? file[%s]\n", __FUNCTION__, file_name.c_str());
        return false;
    }
    schemas_[schema->id] = schema;
    schemas_by_name_[schema->name] = schema;
    XLOG(XLOG_INFO, "tlv_schema_manager::%s load %s\n", __FUNCTION__, file_name.c_str());

    return true;
}
