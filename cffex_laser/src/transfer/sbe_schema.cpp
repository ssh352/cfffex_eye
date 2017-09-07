#include "sbe_schema.h"
#include <cffex/common/dir_reader.h>
#include <cffex/tinyxml2/tinyxml2.h>
#include <cffex/log/loghelper.h>
#include <iostream>

using namespace cffex::common;
using namespace cffex::tinyxml2;

namespace
{

    int sbe_type_len[] = { 0, 1, 1, 1, 2, 2, 4, 4, 8, 8, 4, 8 };

    static class sbe_type_map
    {
    public:
        sbe_type_map()
        {
            map_["char"]   = SBE_CHAR;
            map_["int8"]   = SBE_INT8;
            map_["uint8"]  = SBE_UINT8;
            map_["int16"]  = SBE_INT16;
            map_["uint16"] = SBE_UINT16;
            map_["int32"]  = SBE_INT32;
            map_["uint32"] = SBE_UINT32;
            map_["int64"]  = SBE_INT64;
            map_["uint64"] = SBE_UINT64;
            map_["float"]  = SBE_FLOAT;
            map_["double"] = SBE_DOUBLE;
        }

        int get(const std::string &str)
        {
            std::map<std::string, int>::iterator itr = map_.find(str);
            return itr != map_.end() ? itr->second : 0;
        }

    private:
        std::map<std::string, int> map_;
    } sbe_type_map;

    sbe_type *parse_sbe_type(XMLElement * xmlType)
    {
        sbe_type *result = new sbe_type;
        result->sbe_type_type = SBE_TYPE_TYPE;
        const char *ptr;
        if ((ptr = xmlType->Attribute("name")) == NULL)
        {
           XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.encodingType\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->name = ptr;

        if ((ptr = xmlType->Attribute("primitiveType")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.primitiveType\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->primitive_type = sbe_type_map.get(ptr);
        if (result->primitive_type == 0)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s unknown attribute.primitiveType[%s]\n", __FUNCTION__, ptr);
            delete result;
            return NULL;
        }

        if ((ptr = xmlType->Attribute("length")) != NULL)
        {
            result->length = atoi(ptr);
        }

        if ((ptr = xmlType->Attribute("minValue")) != NULL)
        {
            result->min_value = ptr;
        }
        if ((ptr = xmlType->Attribute("maxValue")) != NULL)
        {
            result->max_value = ptr;
        }

        result->since_version = xmlType->IntAttribute("sinceVersion");

        if ((ptr = xmlType->Attribute("presence")) != NULL)
        {
            if (ptr == std::string("constant"))
            {
                ptr = xmlType->GetText();
                if (ptr != NULL)
                {
                    result->constant_value = ptr;
                }
                result->presence = SBE_PRESENCE_CONSTANT;
            }
        }

        return result;
    }

    sbe_enum *parse_sbe_enum(XMLElement * xmlType)
    {
        sbe_enum *result = new sbe_enum;
        result->sbe_type_type = SBE_TYPE_ENUM;
        const char *ptr;
        if ((ptr = xmlType->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.name\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->name = ptr;

        if ((ptr = xmlType->Attribute("encodingType")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.encodingType\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->encoding_type = sbe_type_map.get(ptr);
        if (result->encoding_type == 0)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s unknown encodingType[%s]\n", __FUNCTION__, ptr);
            delete result;
            return NULL;
        }

        for (XMLElement * xmlValue = xmlType->FirstChildElement("validValue");
            xmlValue != NULL; xmlValue = xmlValue->NextSiblingElement("validValue"))
        {
            const char *name = xmlValue->Attribute("name");
            const char *value = xmlValue->GetText();
            if (name == NULL || value == NULL)
            {
                XLOG(XLOG_ERROR, "sbe_schema_manager::%s validValue node error in[%s]\n", __FUNCTION__, result->name.c_str());
                delete result;
                return NULL;
            }
            int nvalue = (result->encoding_type == SBE_CHAR ? (unsigned char)value[0] : atoi(value));
            //result->values[name] = nvalue;
            result->values_revs[nvalue] = name;
        }
        return result;
    }

    sbe_set *parse_sbe_set(XMLElement * xmlType)
    {
        sbe_set *result = new sbe_set;
        result->sbe_type_type = SBE_TYPE_SET;
        const char *ptr;
        if ((ptr = xmlType->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.name\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->name = ptr;

        if ((ptr = xmlType->Attribute("encodingType")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.encodingType\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->encoding_type = sbe_type_map.get(ptr);
        if (result->encoding_type == 0)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s unknown encodingType[%s]\n", __FUNCTION__, ptr);
            delete result;
            return NULL;
        }

        for (XMLElement * xmlValue = xmlType->FirstChildElement("choice");
            xmlValue != NULL; xmlValue = xmlValue->NextSiblingElement("choice"))
        {
            const char *name = xmlValue->Attribute("name");
            const char *value = xmlValue->GetText();
            if (name == NULL || value == NULL)
            {
                XLOG(XLOG_ERROR, "sbe_schema_manager::%s choice node error in[%s]\n", __FUNCTION__, result->name.c_str());
                delete result;
                return NULL;
            }
            int nvalue = (result->encoding_type == SBE_CHAR ? (unsigned char)value[0] : atoi(value));
            result->choices[name] = nvalue;
            //result->choices_revs[value] = name;
        }
        return result;
    }

    sbe_composite *parse_sbe_composite(XMLElement * xmlType)
    {
        sbe_composite *result = new sbe_composite;
        result->sbe_type_type = SBE_TYPE_COMPOS;
        const char *ptr;
        if ((ptr = xmlType->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.name\n", __FUNCTION__);
            delete result;
            return NULL;
        }
        result->name = ptr;

        for (XMLElement * xmlSub = xmlType->FirstChildElement();
            xmlSub != NULL; xmlSub = xmlSub->NextSiblingElement())
        {
            ptr = xmlSub->Name();
            if (ptr == std::string("type"))
            {
                sbe_type *type = parse_sbe_type(xmlSub);
                if (type == NULL)
                {
                    delete result;
                    return NULL;
                }
                result->types.push_back(type);
            }
            else if (ptr == std::string("ref"))
            {
                const char *sname = xmlSub->Attribute("name");
                const char *stype = xmlSub->Attribute("type");
                if (sname == NULL || stype == NULL)
                {
                    XLOG(XLOG_ERROR, "sbe_schema_manager::%s ref node error\n", __FUNCTION__);
                    return NULL;
                }
                sbe_type *type = new sbe_type;
                type->name = sname;
                type->user_type = stype;
                result->types.push_back(type);
            }
            else if (ptr == std::string("enum"))
            {
                sbe_enum *type = parse_sbe_enum(xmlSub);
                if (type == NULL)
                {
                    delete result;
                    return NULL;
                }
                sbe_type *type2 = new sbe_type;
                type2->name = type->name;
                type2->user_type = type->name;
                type2->inner_type = true;
                result->types.push_back(type2);
                result->inner_types[type->name] = type;
            }
            else if (ptr == std::string("set"))
            {
                sbe_set *type = parse_sbe_set(xmlSub);
                if (type == NULL)
                {
                    delete result;
                    return NULL;
                }
                sbe_type *type2 = new sbe_type;
                type2->name = type->name;
                type2->user_type = type->name;
                type2->inner_type = true;
                result->types.push_back(type2);
                result->inner_types[type->name] = type;
            }
            else if (ptr == std::string("composite"))
            {
                sbe_composite *type = parse_sbe_composite(xmlSub);
                if (type == NULL)
                {
                    delete result;
                    return NULL;
                }
                sbe_type *type2 = new sbe_type;
                type2->name = type->name;
                type2->user_type = type->name;
                type2->inner_type = true;
                result->types.push_back(type2);
                result->inner_types[type->name] = type;
            }
        }

        return result;
    }

    //////////////////////////////////////////////////////////////////////////

    bool parse_sbe_msg_filed(std::vector<sbe_msg_field> &fileds, XMLElement *xmlNode)
    {
        sbe_msg_field filed;
        const char *ptr;
        if ((ptr = xmlNode->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.name\n", __FUNCTION__);
            return false;
        }
        filed.name = ptr;

        if ((ptr = xmlNode->Attribute("type")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute: type\n", __FUNCTION__);
            return false;
        }
        filed.type_name = ptr;
        filed.type = sbe_type_map.get(ptr);
        filed.since_version = xmlNode->IntAttribute("sinceVersion");

        if ((ptr = xmlNode->Attribute("presence")) != NULL)
        {
            if (ptr == std::string("constant"))
            {
                ptr = xmlNode->Attribute("valueRef");
                if (ptr != NULL)
                {
                    filed.value_ref = ptr; // TODO
                    size_t pos = filed.value_ref.find(".");
                    if (pos != std::string::npos)
                        filed.value_ref = filed.value_ref.substr(pos + 1);
                }
                filed.presence = SBE_PRESENCE_CONSTANT;
            }
        }
        fileds.push_back(filed);
        return true;
    }

    bool parse_sbe_msg_data(std::vector<sbe_msg_data> &datas, XMLElement *xmlNode)
    {
        const char *name = xmlNode->Attribute("name");
        const char *type = xmlNode->Attribute("type");
        if (name == NULL || type == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s data node error\n", __FUNCTION__);
            return false;
        }
        sbe_msg_data data;
        data.name = name;
        data.type = type;
        data.since_version = xmlNode->IntAttribute("sinceVersion");
        datas.push_back(data);

        return true;
    }

    bool parse_sbe_msg_group(sbe_schema *schema, std::vector<sbe_msg_group> &groups, XMLElement *xmlNode)
    {
        sbe_msg_group group;
        const char *ptr;
        if ((ptr = xmlNode->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.name\n", __FUNCTION__);
            return false;
        }
        group.name = ptr;

        if ((ptr = xmlNode->Attribute("dimensionType")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s no attribute.dimensionType\n", __FUNCTION__);
            return false;
        }
        group.dimension_type = ptr;

        for (XMLElement *xmlSub = xmlNode->FirstChildElement();
            xmlSub != NULL; xmlSub = xmlSub->NextSiblingElement())
        {
            const char *nodename = xmlSub->Name();
            if (nodename == NULL)
            {
                XLOG(XLOG_ERROR, "sbe_schema_manager::%s get node name failed\n", __FUNCTION__);
                return false;
            }
            if (nodename == std::string("field"))
            {
                if (!parse_sbe_msg_filed(group.fileds, xmlSub))
                {
                    return false;
                }
            }
            else if (nodename == std::string("group"))
            {
                if (!parse_sbe_msg_group(schema, group.groups, xmlSub))
                {
                    return false;
                }
            }
            else if (nodename == std::string("data"))
            {
                if (!parse_sbe_msg_data(group.datas, xmlSub))
                {
                    return false;
                }
            }
        }

        int offset = 0;
        for (std::vector<sbe_msg_field>::iterator itr = group.fileds.begin();
            itr != group.fileds.end(); ++itr)
        {
            sbe_msg_field &field = *itr;
            if (field.presence == SBE_PRESENCE_CONSTANT)
                continue;
            if (field.type != 0)
            {
                field.byte_use = sbe_type_len[field.type];
            }
            else
            {
                std::map<std::string, sbe_type_base *>::iterator itr_type = schema->types.find(field.type_name);
                if (itr_type == schema->types.end())
                {
                    return false;
                }
                field.byte_use = itr_type->second->byte_use;
            }
            field.offset = offset;
            offset += field.byte_use;
        }
        group.block_len = offset;
        //XLOG(XLOG_INFO, "%s block_len: %d\n", group.name.c_str(), group.block_len);
        groups.push_back(group);
        return true;
    }

    //////////////////////////////////////////////////////////////////////////

    bool init_type_use_byte_and_offset(sbe_type_base *type, std::map<std::string, sbe_type_base *> &types)
    {
        if (type->inited)
            return true;

        if (type->sbe_type_type == SBE_TYPE_TYPE)
        {
            sbe_type *type_type = (sbe_type *)type;
            if (type_type->presence == SBE_PRESENCE_CONSTANT)
                return true;

            if (type_type->primitive_type != 0)
            {
                type_type->byte_use = sbe_type_len[type_type->primitive_type] * type_type->length;
            }
            else
            {
                std::map<std::string, sbe_type_base *>::iterator itr = types.find(type_type->user_type);
                if (itr == types.end())
                {
                    XLOG(XLOG_ERROR, "sbe_schema_manager::%s not found user type[%s]\n", __FUNCTION__, type_type->user_type.c_str());
                    return false;
                }
                if (!init_type_use_byte_and_offset(itr->second, types))
                {
                    return false;
                }
                type_type->byte_use = itr->second->byte_use;
            }
        }
        else if (type->sbe_type_type == SBE_TYPE_ENUM)
        {
            sbe_enum *type_enum = (sbe_enum *)type;
            type_enum->byte_use = sbe_type_len[type_enum->encoding_type];
        }
        else if (type->sbe_type_type == SBE_TYPE_SET)
        {
            sbe_set *type_set = (sbe_set *)type;
            type_set->byte_use = sbe_type_len[type_set->encoding_type];
        }
        else if (type->sbe_type_type == SBE_TYPE_COMPOS)
        {
            sbe_composite *type_comp = (sbe_composite *)type;
            int offset = 0;
            type_comp->byte_use = 0;
            for (std::vector<sbe_type *>::iterator itr = type_comp->types.begin();
                itr != type_comp->types.end(); ++itr)
            {
                if ((*itr)->presence == SBE_PRESENCE_CONSTANT)
                    continue;
                if ((*itr)->primitive_type != 0)
                {
                    (*itr)->byte_use = sbe_type_len[(*itr)->primitive_type] * (*itr)->length;
                }
                else
                {
                    std::map<std::string, sbe_type_base *> &the_types = ((*itr)->inner_type ? type_comp ->inner_types: types);
                    std::map<std::string, sbe_type_base *>::iterator itrVec = the_types.find((*itr)->user_type);
                    if (itrVec == the_types.end())
                    {
                        XLOG(XLOG_ERROR, "sbe_schema_manager::%s not found user type[%s]\n", __FUNCTION__, (*itr)->user_type.c_str());
                        return false;
                    }
                    if (!init_type_use_byte_and_offset(itrVec->second, the_types))
                    {
                        return false;
                    }
                    (*itr)->byte_use = itrVec->second->byte_use;
                }
                //std::cout << type_comp->name << "." << (*itr)->name << ", offset=" << offset << std::endl;
                type_comp->byte_use += (*itr)->byte_use;
                (*itr)->offset = offset;
                offset += (*itr)->byte_use;
            }
        }
        type->inited = true;
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////

bool sbe_schema_manager::load(const std::string &dir_name)
{
    dir_reader dr("*.xml");
    char filename[2048] = { 0 };
    if (!dr.open(dir_name.c_str()))
    {
        XLOG(XLOG_ERROR, "sbe_schema_manager::%s open dir failed[%s]\n", __FUNCTION__, dir_name.c_str());
        return false;
    }
    if (!dr.get_first_file_name(filename, sizeof(filename)))
    {
        XLOG(XLOG_WARNING, "sbe_schema_manager::%s get_first_file_name failed[%s]\n", __FUNCTION__, dir_name.c_str());
        return false;
    }
    load_schema(filename);

    while (dr.get_next_file_name(filename, sizeof(filename)))
    {
        load_schema(filename);
    }
    return true;
}

const sbe_schema *sbe_schema_manager::get_schema(int id)
{
    std::map<int, sbe_schema *>::iterator itr = schemas_.find(id);
    return itr != schemas_.end() ? itr->second : NULL;
}

const sbe_schema *sbe_schema_manager::get_schema(const std::string &name)
{
    std::map<std::string, sbe_schema *>::iterator itr = schemas_by_name_.find(name);
    return itr != schemas_by_name_.end() ? itr->second : NULL;
}

void sbe_schema_manager::load_schema(const std::string &file_name)
{
    XMLDocument xmldoc;
    if (xmldoc.LoadFile(file_name.c_str()) != XML_SUCCESS)
    {
        XLOG(XLOG_ERROR, "sbe_schema_manager::%s load xml failed[%s]\n", __FUNCTION__, file_name.c_str());
        return;
    }
    XMLElement *xmlRoot = xmldoc.RootElement();
    if (xmlRoot == NULL)
    {
        XLOG(XLOG_ERROR, "sbe_schema_manager::%s get xml root failed\n", __FUNCTION__);
        return;
    }

    sbe_schema *schema = new sbe_schema;
    schema->id = xmlRoot->IntAttribute("id");
    const char *pname;
    if ((pname = xmlRoot->Attribute("package")) == NULL)
    {
        XLOG(XLOG_ERROR, "sbe_schema_manager::%s schema no attribute.package failed\n", __FUNCTION__);
        return;
    }
    schema->name = pname;
    const char *byte_order = xmlRoot->Attribute("byteOrder");
    schema->version = xmlRoot->IntAttribute("version");
    if (byte_order != NULL && std::string("bigEndian") == byte_order)
        schema->byte_order = SBE_BYTE_ORDER_GIG;

    for (XMLElement * xmlTypes = xmlRoot->FirstChildElement("types");
        xmlTypes != NULL; xmlTypes = xmlTypes->NextSiblingElement("types"))
    {
        for (XMLElement * xmlType = xmlTypes->FirstChildElement();
            xmlType != NULL; xmlType = xmlType->NextSiblingElement())
        {
            const char *nodename = xmlType->Name();
            if (nodename == NULL)
            {
                //std::cout << "get node name failed" << std::endl;
                return;
            }
            sbe_type_base *type = NULL;
            if (nodename == std::string("type"))
            {
                type = parse_sbe_type(xmlType);
            }
            else if (nodename == std::string("enum"))
            {
                type = parse_sbe_enum(xmlType);
            }
            else if (nodename == std::string("set"))
            {
                type = parse_sbe_set(xmlType);
            }
            else if (nodename == std::string("composite"))
            {
                type = parse_sbe_composite(xmlType);
            }

            if (type == NULL) return;
            schema->types[type->name] = type;
        }
    }

    for (std::map<std::string, sbe_type_base *>::iterator itr = schema->types.begin();
        itr != schema->types.end(); ++itr)
    {
        if (!init_type_use_byte_and_offset(itr->second, schema->types))
        {
            return;
        }
    }

    for (XMLElement * xmlMessage = xmlRoot->FirstChildElement("sbe:message");
        xmlMessage != NULL; xmlMessage = xmlMessage->NextSiblingElement("sbe:message"))
    {
        sbe_msg *msg = new sbe_msg;
        const char *ptr;
        if ((ptr = xmlMessage->Attribute("name")) == NULL)
        {
            XLOG(XLOG_ERROR, "sbe_schema_manager::%s sbe:message no attribute.name failed\n", __FUNCTION__);
            return;
        }
        msg->name = ptr;
        msg->id = xmlMessage->IntAttribute("id");
        //std::cout << xmlMessage->Name() << std::endl;

        for (XMLElement * xmlField = xmlMessage->FirstChildElement();
            xmlField != NULL; xmlField = xmlField->NextSiblingElement())
        {
            const char *nodename = xmlField->Name();
            if (nodename == NULL)
            {
                XLOG(XLOG_ERROR, "sbe_schema_manager::%s sbe:message.? get node name failed\n", __FUNCTION__);
                return;
            }
            if (nodename == std::string("field"))
            {
                if (!parse_sbe_msg_filed(msg->fileds, xmlField))
                {
                    return;
                }
            }
            else if (nodename == std::string("group"))
            {
                if (!parse_sbe_msg_group(schema, msg->groups, xmlField))
                {
                    return;
                }
            }
            else if (nodename == std::string("data"))
            {
                if (!parse_sbe_msg_data(msg->datas, xmlField))
                {
                    return;
                }
            }
        }

        int offset = 0;
        for (std::vector<sbe_msg_field>::iterator itr = msg->fileds.begin();
            itr != msg->fileds.end(); ++itr)
        {
            sbe_msg_field &field = *itr;
            if (field.presence == SBE_PRESENCE_CONSTANT)
            {
                field.byte_use = 0;
                continue;
            }
            if (field.type != 0)
            {
                field.byte_use = sbe_type_len[field.type];
            }
            else
            {
                std::map<std::string, sbe_type_base *>::iterator itr_type = schema->types.find(field.type_name);
                if (itr_type == schema->types.end())
                {
                    return;
                }
                field.byte_use = itr_type->second->byte_use;
            }
            //std::cout << field.name << ", offset=" << offset << std::endl;
            field.offset = offset;
            offset += field.byte_use;
        }
        msg->block_len = offset;
        schema->msgs[msg->id] = msg;
        schema->msgs_by_name[msg->name] = msg;
    }

    if (schemas_.find(schema->id) != schemas_.end() || schemas_by_name_.find(schema->name) != schemas_by_name_.end())
    {
        XLOG(XLOG_WARNING, "sbe_schema_manager::%s sbe_schema may be duplicated? [%s]\n", __FUNCTION__, file_name.c_str());
    }
    schemas_[schema->id] = schema;
    schemas_by_name_[schema->name] = schema;
    XLOG(XLOG_INFO, "sbe_schema_manager::%s load [%s]\n", __FUNCTION__, file_name.c_str());
}
