#include "sbe_transfer.h"
#include "sbe_schema.h"
#include "jsoncoder.h"
#include <cffex/common/string_helper.h>
#include <json-c/json.h>
#include "error_info.h"

namespace
{

uint16_t endian_swap(int byte_order, uint16_t v)
{
    return byte_order == SBE_BYTE_ORDER_LITTLE ? v : g_htons(v);
}

uint32_t endian_swap(int byte_order, uint32_t v)
{
    return byte_order == SBE_BYTE_ORDER_LITTLE ? v : g_htonl(v);
}

uint64_t endian_swap(int byte_order, uint64_t v)
{
    return byte_order == SBE_BYTE_ORDER_LITTLE ? v : g_htonll(v);
}

void set_int_value(int byte_order, int field_type, char *ptr, int value)
{
    if (field_type == SBE_CHAR || field_type == SBE_INT8)
    {
        *(char*)(ptr) = (int8_t)value;
    }
    else if (field_type == SBE_UINT8)
    {
        *(uint8_t*)(ptr) = (uint8_t)value;
    }
    else if (field_type == SBE_INT16 || field_type == SBE_UINT16)
    {
        int16_t t = value;
        *(uint16_t*)(ptr) = endian_swap(byte_order, (uint16_t)t);
    }
    else if (field_type == SBE_INT32 || field_type == SBE_UINT32)
    {
        *(uint32_t*)(ptr) = endian_swap(byte_order, (uint32_t)value);
    }
}

void set_sbe_type(int byte_order, int field_type, int field_count, char *ptr, json_object *json)
{
    if (field_count == 0 || json == NULL)
        return;

    if (field_type == SBE_CHAR)
    {
        const char *str = json_object_get_string(json);
        int len = strlen(str);
        if (len > field_count) len = field_count;
        memcpy(ptr, str, len);
        return;
    }
    else
    {
        if (field_type == SBE_INT8)
        {
            *(char*)(ptr) = (int8_t)json_object_get_int(json);
        }
        else if (field_type == SBE_UINT8)
        {
            *(char*)(ptr) = (uint8_t)json_object_get_int(json);
        }
        else if (field_type == SBE_INT16)
        {
            *(uint16_t*)(ptr) = endian_swap(byte_order, (uint16_t)(int16_t)json_object_get_int(json));
        }
        else if (field_type == SBE_UINT16)
        {
            *(uint16_t*)(ptr) = endian_swap(byte_order, (uint16_t)json_object_get_int(json));
        }
        else if (field_type == SBE_INT32 || field_type == SBE_UINT32)
        {
            *(uint32_t*)(ptr) = endian_swap(byte_order, (uint32_t)json_object_get_int(json));
        }
        else if (field_type == SBE_INT64 || field_type == SBE_UINT64)
        {
            *(uint64_t*)(ptr) = endian_swap(byte_order, (uint64_t)json_object_get_int64(json));
        }
        else if (field_type == SBE_FLOAT)
        {
            float fv = json_object_get_double(json);
            uint32_t v = *(uint32_t *)&fv;
            *(uint32_t*)(ptr) = endian_swap(byte_order, v);
        }
        else if (field_type == SBE_DOUBLE)
        {
            double fv = json_object_get_double(json);
            uint64_t v = *(uint64_t *)&fv;
            *(uint64_t*)(ptr) = endian_swap(byte_order, v);
        }
    }
}

} // namespace

int sbe_transfer::http_to_xp(const char *service, const char *message,
        const std::map<std::string, std::string> &params, xp_encoder *e)
{
    XLOG(XLOG_DEBUG, "sbe_transfer::%s service[%s], message[%s]\n", __FUNCTION__, service, message);

    auto params_itr = params.find("params");
    if (params_itr == params.end())
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get params failed, service[%s]\n", __FUNCTION__, service);
        return ERROR_BAD_PARAM;
    }
    const sbe_schema *schema = sbe_schema_manager::get_instance()->get_schema(service);
    if (schema == NULL)
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get schema failed, service[%s]\n", __FUNCTION__, service);
        return ERROR_NO_REQ_SERVICE;
    }
    std::map<std::string, sbe_msg *>::const_iterator itrMsg = schema->msgs_by_name.find(message);
    if (itrMsg == schema->msgs_by_name.end())
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get schema failed, service[%s], message[%s]\n", __FUNCTION__, service, message);
        return ERROR_NO_REQ_MESSAGE;
    }

    e->set_proto_type(PT_SBE);
    e->set_serviceid(schema->id);
    e->set_messageid(itrMsg->second->id);

    // get value from sbe header
    std::map<std::string, sbe_type_base *>::const_iterator itrHd = schema->types.find("messageHeader");
    if (itrHd == schema->types.end() || itrHd->second->sbe_type_type != SBE_TYPE_COMPOS)
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get messageHeader failed, service[%s]\n", __FUNCTION__, service);
        return ERROR_REQ_SCHEMA;
    }

    json_array_wrapper json_array;
    if (!json_array.parse(params_itr->second.c_str()))
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s parse json failed, json[%s]\n", __FUNCTION__, params_itr->second.c_str());
        return ERROR_BAD_PARAM;
    }

    cffex::buffer *buffer = e->get_buffer();
    const sbe_composite *header = (const sbe_composite *)itrHd->second;
    for (std::vector<sbe_type *>::const_iterator itr = header->types.begin(); itr != header->types.end(); ++itr)
    {
        int v = 0;
        if ((*itr)->name == "blockLength")
            v = itrMsg->second->block_len;
        else if((*itr)->name == "version")
            v = schema->version;
        set_int_value(schema->byte_order, (*itr)->primitive_type, buffer->top() + (*itr)->offset, v);
    }
    buffer->inc_loc(header->byte_use);

    for (std::vector<sbe_msg_field>::const_iterator itr = itrMsg->second->fileds.begin(); itr != itrMsg->second->fileds.end(); ++itr)
    {
        const sbe_msg_field &field = *itr;
        if (field.type != SBE_USER) // base type
        {
            if (field.presence == SBE_PRESENCE_CONSTANT)
                continue;
            set_sbe_type(schema->byte_order, field.type, 1, buffer->top() + field.offset, json_array.get_next());
        }
        else
        {
            std::map<std::string, sbe_type_base *>::const_iterator itrT = schema->types.find(field.type_name);
            if (itrT == schema->types.end())
            {
                XLOG(XLOG_ERROR, "sbe_transfer::%s get field.type_name failed[%s] %s\n", __FUNCTION__, field.name.c_str(), field.type_name.c_str());
                return ERROR_REQ_SCHEMA;
            }
            if (itrT->second->sbe_type_type == SBE_TYPE_TYPE)
            {
                const sbe_type *type = (const sbe_type *)itrT->second;
                if (type->primitive_type != SBE_USER)
                {
                    if (field.presence == SBE_PRESENCE_CONSTANT || type->presence == SBE_PRESENCE_CONSTANT)
                        continue;
                    if (type->primitive_type != SBE_CHAR && type->length > 1)
                    {
                        XLOG(XLOG_ERROR, "sbe_transfer::%s unsupported sbe encode type[%s %s]\n", __FUNCTION__, field.name.c_str(), field.type_name.c_str());
                        return ERROR_REQ_SCHEMA;
                    }
                    set_sbe_type(schema->byte_order, type->primitive_type, type->length, buffer->top() + field.offset, json_array.get_next());
                }
                else
                {
                    XLOG(XLOG_ERROR, "sbe_transfer::%s unsupported sbe encode type[%s %s]\n", __FUNCTION__, field.name.c_str(), field.type_name.c_str());
                    return ERROR_REQ_SCHEMA;
                }
            }
            else
            {
                XLOG(XLOG_ERROR, "%s unsupported sbe encode type[%s] %s\n", __FUNCTION__, field.name.c_str(), field.type_name.c_str());
                return ERROR_REQ_SCHEMA;
            }
        }
    }
    buffer->inc_loc(itrMsg->second->block_len);

    if (itrMsg->second->groups.size() > 0)
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s unsupported sbe encode type: group\n", __FUNCTION__);
        return ERROR_REQ_SCHEMA;
    }

    for (std::vector<sbe_msg_data>::const_iterator itr = itrMsg->second->datas.begin(); itr != itrMsg->second->datas.end(); ++itr)
    {
        const sbe_msg_data &data = *itr;
        std::map<std::string, sbe_type_base *>::const_iterator itrDim = schema->types.find(data.type);
        if (itrDim == schema->types.end() || itrDim->second->sbe_type_type != SBE_TYPE_COMPOS)
        {
            XLOG(XLOG_ERROR, "sbe_transfer::%s get data.dimension_type failed[%s %s]\n", __FUNCTION__, data.name.c_str(), data.type.c_str());
            return ERROR_REQ_SCHEMA;
        }

        int length = 0;
        const char *value = NULL;
        json_object * json_str = json_array.get_next();
        if (json_str != NULL && (value = json_object_get_string(json_str)) != NULL)
        {
            length = strlen(value);
        }
        const sbe_composite *dimension = (const sbe_composite *)itrDim->second; // varStringEncoding varDataEncoding
        for (std::vector<sbe_type *>::const_iterator itr = dimension->types.begin(); itr != dimension->types.end(); ++itr)
        {
            if ((*itr)->length == 0) continue;
            int v = 0;
            if ((*itr)->name == "length")
                v = length;
            set_int_value(schema->byte_order, (*itr)->primitive_type, buffer->top() + (*itr)->offset, v);
        }
        buffer->inc_loc(dimension->byte_use);
        if (length > 0)
        {
            memcpy(buffer->top(), value, length);
            buffer->inc_loc(length);
        }
    }

    e->set_len();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

namespace
{

int get_int_value(int byte_order, int field_type, const char *ptr)
{
    if (field_type == SBE_CHAR || field_type == SBE_INT8 || field_type == SBE_UINT8)
    {
        char v = *(const char*)(ptr);
        return (int16_t)v;
    }
    else if (field_type == SBE_INT16 || field_type == SBE_UINT16)
    {
        uint16_t v = *(const uint16_t*)(ptr);
        return endian_swap(byte_order, v);
    }
    else if (field_type == SBE_INT32 || field_type == SBE_UINT32)
    {
        uint32_t v = *(const uint32_t*)(ptr);
        return  endian_swap(byte_order, v);
    }
    else
    {
        return 0;
    }
}

void set_json_type_const(int field_type, const std::string &field_name, const std::string &value, json_encoder &json)
{
    if (field_type == SBE_CHAR)
    {
        json.set_value(field_name, value);
    }
    else if (field_type == SBE_INT8 || field_type == SBE_UINT8 ||
        field_type == SBE_INT16 || field_type == SBE_UINT16 ||
        field_type == SBE_INT32 || field_type == SBE_UINT32)
    {
        json.set_value(field_name, atoi(value.c_str()));
    }
    else if (field_type == SBE_INT64 || field_type == SBE_UINT64)
    {
        json.set_value(field_name, (long long)atol(value.c_str()));
    }
    else
    {
        json.set_value(field_name, (int)atof(value.c_str()));
    }
}

void set_json_type(int byte_order, int field_type, const std::string &field_name, int field_count, const char *ptr, json_encoder &json)
{
    if (field_count == 0) return;
    if (field_type == SBE_CHAR)
    {
        json.set_value(field_name, std::string(ptr, field_count));
        return;
    }

    if (field_count == 1)
    {
        if (field_type == SBE_INT8)
        {
            json.set_value(field_name, (int)*(const char*)(ptr));
        }
        else if (field_type == SBE_UINT8)
        {
            json.set_value(field_name, (int)*(const unsigned char*)(ptr));
        }
        else if (field_type == SBE_INT16 )
        {
            json.set_value(field_name, (int)(int16_t)endian_swap(byte_order, *(const uint16_t*)(ptr)));
        }
        else if (field_type == SBE_UINT16)
        {
            json.set_value(field_name, (int)endian_swap(byte_order, *(const uint16_t*)(ptr)));
        }
        else if (field_type == SBE_INT32 || field_type == SBE_UINT32)
        {
            json.set_value(field_name, (int)endian_swap(byte_order, *(const uint32_t*)(ptr)));
        }
        else if (field_type == SBE_INT64 || field_type == SBE_UINT64)
        {
            json.set_value(field_name, (long long)endian_swap(byte_order, *(const uint64_t*)(ptr)));
        }
        else if (field_type == SBE_FLOAT)
        {
            uint32_t v = endian_swap(byte_order, *(const uint32_t*)(ptr));
            json.set_value(field_name, (double)*(float *)&v);
        }
        else if (field_type == SBE_DOUBLE)
        {
            uint64_t v = endian_swap(byte_order, *(const uint64_t*)(ptr));
            json.set_value(field_name, *(double *)&v);
        }
        else
        {
            json.set_value(field_name, field_type);
        }
    }
    else
    {
        if (field_type == SBE_INT8)
        {
            std::vector<int> v;
            for (int i = 0; i < field_count; ++i)
            {
                v.push_back(*(const char*)(ptr + i));
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_UINT8)
        {
            std::vector<int> v;
            for (int i = 0; i < field_count; ++i)
            {
                v.push_back(*(const unsigned char*)(ptr + i));
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_INT16)
        {
            std::vector<int> v;
            for (int i = 0; i < field_count; ++i)
            {
                v.push_back((int16_t)endian_swap(byte_order, *(const uint16_t*)(ptr + 2 * i)));
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_UINT16)
        {
            std::vector<int> v;
            for (int i = 0; i < field_count; ++i)
            {
                v.push_back(endian_swap(byte_order, *(const uint16_t*)(ptr + 2 * i)));
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_INT32 || field_type == SBE_UINT32)
        {
            std::vector<int> v;
            for (int i = 0; i < field_count; ++i)
            {
                v.push_back(endian_swap(byte_order, *(const uint32_t*)(ptr + 4 * i)));
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_INT64 || field_type == SBE_UINT64)
        {
            std::vector<long long> v;
            for (int i = 0; i < field_count; ++i)
            {
                v.push_back(endian_swap(byte_order, *(const uint64_t*)(ptr + 8 * i)));
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_FLOAT)
        {
            std::vector<double> v;
            for (int i = 0; i < field_count; ++i)
            {
                uint32_t val = endian_swap(byte_order, *(const uint32_t*)(ptr));
                v.push_back((double)*(float *)&val);
            }
            json.set_value(field_name, v);
        }
        else if (field_type == SBE_DOUBLE)
        {
            std::vector<double> v;
            for (int i = 0; i < field_count; ++i)
            {
                uint64_t val = endian_swap(byte_order, *(const uint64_t*)(ptr));
                v.push_back(*(double *)&val);
            }
            json.set_value(field_name, v);
        }
        else
        {
            json.set_value(field_name, field_type);
        }
    }
}

void set_json_enum(int byte_order, const std::string &field_name, const sbe_enum *sbe_enum, const char *ptr, json_encoder &json)
{
    int value = get_int_value(byte_order, sbe_enum->encoding_type, ptr);
    std::map<int, std::string>::const_iterator itr = sbe_enum->values_revs.find(value);
    if (itr != sbe_enum->values_revs.end())
    {
        json.set_value(field_name, /*sbe_enum->name + "." + */itr->second);
    }
}

void set_json_set(int byte_order, const std::string &field_name, const sbe_set *sbe_set, const char *ptr, json_encoder &json)
{
    uint32_t value = get_int_value(byte_order, sbe_set->encoding_type, ptr);
    std::vector<json_encoder> v;
    for (std::map<std::string, int>::const_iterator itr = sbe_set->choices.begin();
        itr != sbe_set->choices.end(); ++itr)
    {
        json_encoder j;
        j.set_value(itr->first, (bool)(value & (0x1L << itr->second)));
        v.push_back(j);
    }
    if (!v.empty())
    {
        json.set_value(field_name, v);
    }
}

void set_json_composite(const sbe_schema *schema, const std::string &field_name, const sbe_composite *sbe_compos, const char *ptr, json_encoder &json)
{
    json_encoder json_com;
    for (std::vector<sbe_type *>::const_iterator itr = sbe_compos->types.begin();
        itr != sbe_compos->types.end(); ++itr)
    {
        const sbe_type * sbetype = *itr;

        if (sbetype->primitive_type != SBE_USER)
        {
            if (sbetype->presence == SBE_PRESENCE_CONSTANT)
                set_json_type_const(sbetype->primitive_type, sbetype->name, sbetype->constant_value, json_com);
            else
                set_json_type(schema->byte_order, sbetype->primitive_type, sbetype->name, sbetype->length,
                 ptr + sbetype->offset, json_com);
        }
        else
        {
            std::map<std::string, sbe_type_base *>::const_iterator itrT =
                sbetype->inner_type ? sbe_compos->inner_types.find(sbetype->user_type): schema->types.find(sbetype->user_type);
            if (itrT == (sbetype->inner_type ? sbe_compos->inner_types.end(): schema->types.end()))
            {
                XLOG(XLOG_ERROR, "%s get sbe_type->user_type failed[%s %s]\n", __FUNCTION__, sbetype->name.c_str(), sbetype->user_type.c_str());
                continue;
            }

            if (itrT->second->sbe_type_type == SBE_TYPE_TYPE)
            {
                const sbe_type *type = (const sbe_type *)itrT->second;
                if (type->primitive_type != SBE_USER)
                {
                    if (type->presence == SBE_PRESENCE_CONSTANT)
                        set_json_type_const(type->primitive_type, sbetype->name, type->constant_value, json_com);
                    else
                        set_json_type(schema->byte_order, type->primitive_type, sbetype->name, type->length,
                         ptr + sbetype->offset, json_com);
                }
            }
            else if (itrT->second->sbe_type_type == SBE_TYPE_ENUM)
            {
                const sbe_enum *type = (const sbe_enum *)itrT->second;
                set_json_enum(schema->byte_order, sbetype->name, type, ptr + sbetype->offset, json_com);
            }
            else if (itrT->second->sbe_type_type == SBE_TYPE_SET)
            {
                const sbe_set *type = (const sbe_set *)itrT->second;
                set_json_set(schema->byte_order, sbetype->name, type, ptr + sbetype->offset, json_com);
            }
            else if (itrT->second->sbe_type_type == SBE_TYPE_COMPOS)
            {
                const sbe_composite *type = (const sbe_composite *)itrT->second;
                set_json_composite(schema, sbetype->name, type, ptr + sbetype->offset, json_com);
            }
        }
    }
    json.set_value(field_name, json_com);
}

void process_msg_fields(int version, const sbe_schema *schema, const std::vector<sbe_msg_field> &fileds, const char *ptr, json_encoder &json_data)
{
    int offset = 0;
    for (std::vector<sbe_msg_field>::const_iterator itr = fileds.begin(); itr != fileds.end(); ++itr)
    {
        const sbe_msg_field &field = *itr;

        if (version < field.since_version) continue;

        if (field.type != SBE_USER) // base type
        {
            if (field.presence == SBE_PRESENCE_CONSTANT)
                set_json_type_const(field.type, field.name, field.value_ref, json_data);
            else
                set_json_type(schema->byte_order, field.type, field.name, 1, ptr + offset, json_data);
        }
        else
        {
            std::map<std::string, sbe_type_base *>::const_iterator itrT = schema->types.find(field.type_name);
            if (itrT == schema->types.end())
            {
                XLOG(XLOG_ERROR, "sbe_transfer::%s get field.type_name failed[%s %s]\n", __FUNCTION__, field.name.c_str(), field.type_name.c_str());
                return;
            }
            if (itrT->second->sbe_type_type == SBE_TYPE_TYPE)
            {
                const sbe_type *type = (const sbe_type *)itrT->second;
                if (type->primitive_type != SBE_USER)
                {
                    if (field.presence == SBE_PRESENCE_CONSTANT)
                        set_json_type_const(type->primitive_type, field.name, field.value_ref, json_data);
                    else if (type->presence == SBE_PRESENCE_CONSTANT)
                        set_json_type_const(type->primitive_type, field.name, type->constant_value, json_data);
                    else
                        set_json_type(schema->byte_order, type->primitive_type, field.name, type->length,
                         ptr + offset, json_data);
                }
            }
            else if (itrT->second->sbe_type_type == SBE_TYPE_ENUM)
            {
                if (field.presence == SBE_PRESENCE_CONSTANT)
                {
                    json_data.set_value(field.name, field.value_ref);
                }
                else
                {
                    const sbe_enum *type = (const sbe_enum *)itrT->second;
                    set_json_enum(schema->byte_order, field.name, type, ptr + offset, json_data);
                }
            }
            else if (itrT->second->sbe_type_type == SBE_TYPE_SET)
            {
                const sbe_set *type = (const sbe_set *)itrT->second;
                set_json_set(schema->byte_order, field.name, type, ptr + offset, json_data);
            }
            else if (itrT->second->sbe_type_type == SBE_TYPE_COMPOS)
            {
                const sbe_composite *type = (const sbe_composite *)itrT->second;
                set_json_composite(schema, field.name, type, ptr + offset, json_data);
            }
        }
        offset += field.byte_use;
    }
}

int process_msg_datas(int version, const sbe_schema *schema, const std::vector<sbe_msg_data> &datas, const char *ptr, json_encoder &json_data)
{
    int offset = 0;
    for (std::vector<sbe_msg_data>::const_iterator itr = datas.begin(); itr != datas.end(); ++itr)
    {
        const sbe_msg_data &data = *itr;
        std::map<std::string, sbe_type_base *>::const_iterator itrDim = schema->types.find(data.type);
        if (itrDim == schema->types.end() || itrDim->second->sbe_type_type != SBE_TYPE_COMPOS)
        {
            XLOG(XLOG_ERROR, "sbe_transfer::%s get data.dimension_type failed[%s %s]\n", __FUNCTION__, data.name.c_str(), data.type.c_str());
            return -1;
        }

        if (version < data.since_version) continue;

        int length = 0;
        const sbe_composite *dimension = (const sbe_composite *)itrDim->second; // varStringEncoding varDataEncoding
        for (std::vector<sbe_type *>::const_iterator itr = dimension->types.begin(); itr != dimension->types.end(); ++itr)
        {
            if ((*itr)->length == 0) continue;
            int v = get_int_value(schema->byte_order, (*itr)->primitive_type, ptr + offset + (*itr)->offset);
            if ((*itr)->name == "length")
                length = v;
        }
        offset += dimension->byte_use;
        if (length <= 0) continue;
        json_data.set_value(data.name, std::string(ptr + offset, length));
        offset += length;
    }
    return offset;
}

int process_msg_groups(int version, const sbe_schema *schema, const std::vector<sbe_msg_group> &groups, const char *ptr, json_encoder &json_data)
{
    int offset = 0;
    for (std::vector<sbe_msg_group>::const_iterator itr = groups.begin(); itr != groups.end(); ++itr)
    {
        const sbe_msg_group &group = *itr;
        std::map<std::string, sbe_type_base *>::const_iterator itrDim = schema->types.find(group.dimension_type);
        if (itrDim == schema->types.end() || itrDim->second->sbe_type_type != SBE_TYPE_COMPOS)
        {
            XLOG(XLOG_ERROR, "sbe_transfer::%s get group.dimension_type failed[%s %s]\n", __FUNCTION__, group.name.c_str(), group.dimension_type.c_str());
            return -1;
        }

        int numInGroup = 0;
        int blockLength = 0;
        const sbe_composite *dimension = (const sbe_composite *)itrDim->second; // groupSizeEncoding
        for (std::vector<sbe_type *>::const_iterator itr = dimension->types.begin(); itr != dimension->types.end(); ++itr)
        {
            int v = get_int_value(schema->byte_order, (*itr)->primitive_type, ptr + offset + (*itr)->offset);
            if ((*itr)->name == "numInGroup")
                numInGroup = v;
            else if((*itr)->name == "blockLength")
                blockLength = v;
        }
        offset += dimension->byte_use;

        std::vector<json_encoder> jsonv;
        for (int i = 0; i < numInGroup; ++i)
        {
            json_encoder json;

            process_msg_fields(version, schema, group.fileds, ptr + offset, json);
            offset += blockLength;
            int len = process_msg_groups(version, schema, group.groups, ptr + offset, json);
            if (len < 0) { return -1; }
            offset += len;
            len = process_msg_datas(version, schema, group.datas, ptr + offset, json);
            if (len < 0) { return -1; }
            offset += len;

            jsonv.push_back(json);
        }

        json_data.set_value(group.name, jsonv);
    }
    return offset;
}

} // namespace

int sbe_transfer::xp_to_http(xp_decoder *d, cffex::http::http_encoder *e)
{
    XLOG(XLOG_DEBUG, "sbe_transfer::%s service[%d], message[%d]\n", __FUNCTION__, d->get_serviceid(), d->get_messageid());

    json_encoder json_root(true);
    const sbe_schema *schema = sbe_schema_manager::get_instance()->get_schema(d->get_serviceid());
    if (schema == NULL)
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get schema failed, serviceid[%d]\n", __FUNCTION__, d->get_serviceid());
        return ERROR_NO_RSP_SERVICE;
    }
    std::map<int, sbe_msg *>::const_iterator itrMsg = schema->msgs.find(d->get_messageid());
    if (itrMsg == schema->msgs.end())
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get schema failed, serviceid[%d], messageid[%d]\n", __FUNCTION__, d->get_serviceid(), d->get_messageid());
        return ERROR_NO_RSP_MESSAGE;
    }

    // get value from sbe header
    std::map<std::string, sbe_type_base *>::const_iterator itrHd = schema->types.find("messageHeader");
    if (itrHd == schema->types.end() || itrHd->second->sbe_type_type != SBE_TYPE_COMPOS)
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s get messageHeader failed, serviceid[%d]\n", __FUNCTION__, d->get_serviceid());
        return ERROR_RSP_SCHEMA;
    }

    int version = 0;
    int blocklen = 0;
    const sbe_composite *header = (const sbe_composite *)itrHd->second;
    for (std::vector<sbe_type *>::const_iterator itr = header->types.begin(); itr != header->types.end(); ++itr)
    {
        int v = get_int_value(schema->byte_order, (*itr)->primitive_type, d->get_body() + (*itr)->offset);
        if ((*itr)->name == "blockLength")
            blocklen = v;
        else if((*itr)->name == "version")
            version = v;
    }
    //if (blocklen == 0)
    //    blocklen = itrMsg->second->block_len

    // get value from sbe body
    const char *sbe_body = d->get_body() + header->byte_use;
    //int sbe_len = d->get_body_len() - header->byte_use;

    json_root.set_value("version", version);
    json_root.set_value("errorCode", (int)d->get_resultcode());

    process_msg_fields(version, schema, itrMsg->second->fileds, sbe_body, json_root);
    int len = process_msg_groups(version, schema, itrMsg->second->groups, sbe_body + blocklen, json_root);
    if (len >= 0)
    {
        process_msg_datas(version, schema, itrMsg->second->datas, sbe_body + blocklen + len, json_root);
    }

    e->body(json_root.to_string());

    return 0;
}
