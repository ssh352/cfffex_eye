#include "tlv_transfer.h"
#include "tlv_schema.h"
#include "jsoncoder.h"
#include "tlv.h"
#include "error_info.h"

int tlv_transfer::http_to_xp(const char *service, const char *message,
        const std::map<std::string, std::string> &params, xp_encoder *e)
{
    XLOG(XLOG_DEBUG, "tlv_transfer::%s service[%s], message[%s]\n", __FUNCTION__, service, message);

    auto params_itr = params.find("params");
    if (params_itr == params.end())
    {
        XLOG(XLOG_ERROR, "tlv_transfer::%s get params failed, service[%s], message[%s]\n", __FUNCTION__, service, message);
        return ERROR_BAD_PARAM;
    }
    json_array_wrapper json_array;
    if (!json_array.parse(params_itr->second.c_str()))
    {
        XLOG(XLOG_ERROR, "sbe_transfer::%s parse json failed, service[%s], message[%s], json[%s]\n", __FUNCTION__, service, message, params_itr->second.c_str());
        return ERROR_BAD_PARAM;
    }

    const tlv_schema *schema = tlv_schema_manager::get_instance()->get_schema(service);
    if (schema == NULL)
    {
        XLOG(XLOG_ERROR, "tlv_transfer::%s get schema failed, service[%s], message[%s]\n", __FUNCTION__, service, message);
        return ERROR_NO_REQ_SERVICE;
    }
    std::map<std::string, tlv_message *>::const_iterator itr_msg = schema->msgs_by_name.find(message);
    if (itr_msg == schema->msgs_by_name.end())
    {
        XLOG(XLOG_ERROR, "tlv_transfer::%s get schema failed, service[%s], message[%s]\n", __FUNCTION__, service, message);
        return ERROR_NO_REQ_MESSAGE;
    }

    e->set_proto_type(PT_TLV);
    e->set_serviceid(schema->id);
    e->set_messageid(itr_msg->second->id);

    tlv_encoder tlven(*e->get_buffer());

    for (auto ifield = itr_msg->second->fields.begin(); ifield != itr_msg->second->fields.end(); ++ifield)
    {
        json_object *json = json_array.get_next();
        if (json == NULL) continue;

        if (ifield->type == TLV_TYPE_USERDEF)
        {
            XLOG(XLOG_ERROR, "tlv_transfer::%s request unsupport user type, service[%s], message[%s]\n", __FUNCTION__, service, message);
            return ERROR_REQ_SCHEMA;
        }
        else if (ifield->type == TLV_TYPE_UINT32)
        {
            tlven.set_value(ifield->id, (uint32_t)json_object_get_int(json));
        }
        else if (ifield->type == TLV_TYPE_INT32)
        {
            tlven.set_value(ifield->id, (int32_t)json_object_get_int(json));
        }
        else if (ifield->type == TLV_TYPE_UINT64)
        {
            tlven.set_value(ifield->id, (uint64_t)json_object_get_int64(json));
        }
        else if (ifield->type == TLV_TYPE_INT64)
        {
            tlven.set_value(ifield->id, (int64_t)json_object_get_int64(json));
        }
        else if (ifield->type == TLV_TYPE_FLOAT)
        {
            tlven.set_value(ifield->id, (float)json_object_get_double(json));
        }
        else if (ifield->type == TLV_TYPE_DOUBLE)
        {
            tlven.set_value(ifield->id, (double)json_object_get_double(json));
        }
        else if (ifield->type == TLV_TYPE_STRING)
        {
            const char *ptr = json_object_get_string(json);
            if (ptr != NULL)
            {
                tlven.set_value(ifield->id, ptr);
            }
        }
    }

    return 0;
}

static void process_tlv_fields(const tlv_schema *schema, const tlv_message *msg, tlv_decoder &tlvde, json_encoder &json)
{
    for (auto field = msg->fields.begin(); field != msg->fields.end(); ++field)
    {
        if (!field->is_array)
        {
            if (field->type == TLV_TYPE_UINT32)
            {
                uint32_t v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, (int32_t)v);
            }
            else if (field->type == TLV_TYPE_INT32)
            {
                int32_t v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, v);
            }
            else if (field->type == TLV_TYPE_UINT64)
            {
                uint64_t v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, (long long)v);
            }
            else if (field->type == TLV_TYPE_INT64)
            {
                int64_t v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, (long long)v);
            }
            else if (field->type == TLV_TYPE_FLOAT)
            {
                float v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, (double)v);
            }
            else if (field->type == TLV_TYPE_DOUBLE)
            {
                double v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, v);
            }
            else if (field->type == TLV_TYPE_STRING)
            {
                std::string v;
                if (tlvde.get_value(field->id, v)) json.set_value(field->name, v);
            }
            else if (field->type == TLV_TYPE_USERDEF)
            {
                auto itr_user_type = schema->msgs_by_name.find(field->type_name);
                tlv_decoder tlvde_sub;
                if (itr_user_type != schema->msgs_by_name.end() && tlvde.get_value(field->id, tlvde_sub))
                {
                    json_encoder json_sub;
                    process_tlv_fields(schema, itr_user_type->second, tlvde_sub, json_sub);
                    json.set_value(field->name, json_sub);
                }
            }
        }
        else
        {
            if (field->type == TLV_TYPE_UINT32)
            {
                std::vector<uint32_t> values;
                if (tlvde.get_value(field->id, values))
                {
                    std::vector<int32_t> v2;
                    for (auto i = values.begin(); i != values.end(); ++i) v2.push_back(*i);
                    json.set_value(field->name, v2);
                }
            }
            else if (field->type == TLV_TYPE_INT32)
            {
                std::vector<int32_t> values;
                if (tlvde.get_value(field->id, values))
                {
                    json.set_value(field->name, values);
                }
            }
            else if (field->type == TLV_TYPE_UINT64)
            {
                std::vector<uint64_t> values;
                if (tlvde.get_value(field->id, values))
                {
                    std::vector<long long> v2;
                    for (auto i = values.begin(); i != values.end(); ++i) v2.push_back(*i);
                    json.set_value(field->name, v2);
                }
            }
            else if (field->type == TLV_TYPE_INT64)
            {
                std::vector<int64_t> values;
                if (tlvde.get_value(field->id, values))
                {
                    std::vector<long long> v2;
                    for (auto i = values.begin(); i != values.end(); ++i) v2.push_back(*i);
                    json.set_value(field->name, v2);
                }
            }
            else if (field->type == TLV_TYPE_FLOAT)
            {
                std::vector<float> values;
                if (tlvde.get_value(field->id, values))
                {
                    std::vector<double> v2;
                    for (auto i = values.begin(); i != values.end(); ++i) v2.push_back(*i);
                    json.set_value(field->name, v2);
                }
            }
            else if (field->type == TLV_TYPE_DOUBLE)
            {
                std::vector<double> values;
                if (tlvde.get_value(field->id, values))
                {
                    json.set_value(field->name, values);
                }
            }
            else if (field->type == TLV_TYPE_STRING)
            {
                std::vector<std::string> values;
                if (tlvde.get_value(field->id, values))
                {
                    json.set_value(field->name, values);
                }
            }
            else if (field->type == TLV_TYPE_USERDEF)
            {
                auto itr_user_type = schema->msgs_by_name.find(field->type_name);
                std::vector<tlv_decoder> tlvde_sub;
                if (itr_user_type != schema->msgs_by_name.end() && tlvde.get_value(field->id, tlvde_sub))
                {
                    std::vector<json_encoder> json_subs;
                    for (auto itr_tlv_sub = tlvde_sub.begin(); itr_tlv_sub != tlvde_sub.end(); ++itr_tlv_sub)
                    {
                        json_encoder json_sub;
                        process_tlv_fields(schema, itr_user_type->second, *itr_tlv_sub, json_sub);
                        json_subs.push_back(json_sub);
                    }
                    json.set_value(field->name, json_subs);
                }
            }
        }
    }
}

int tlv_transfer::xp_to_http(xp_decoder *d, cffex::http::http_encoder *e)
{
    XLOG(XLOG_DEBUG, "tlv_transfer::%s service[%d], message[%d]\n", __FUNCTION__, d->get_serviceid(), d->get_messageid());

    json_encoder json_root(true);
    const tlv_schema *schema = tlv_schema_manager::get_instance()->get_schema(d->get_serviceid());
    if (schema == NULL)
    {
        XLOG(XLOG_ERROR, "tlv_transfer::%s get schema failed, service[%d], message[%d]\n",
            __FUNCTION__, d->get_serviceid(), d->get_messageid());
        return ERROR_NO_RSP_SERVICE;
    }
    std::map<int, tlv_message *>::const_iterator itrMsg = schema->msgs.find(d->get_messageid());
    if (itrMsg == schema->msgs.end())
    {
        XLOG(XLOG_ERROR, "tlv_transfer::%s get schema failed, service[%d], message[%d]\n",
            __FUNCTION__, d->get_serviceid(), d->get_messageid());
        return ERROR_NO_RSP_MESSAGE;
    }

    json_root.set_value("version",(int) d->get_version());
    json_root.set_value("errorCode", (int)d->get_resultcode());

    tlv_decoder tlvde(d->get_body(), d->get_body_len());
    process_tlv_fields(schema, itrMsg->second, tlvde, json_root);

    e->body(json_root.to_string());

    return 0;
}
