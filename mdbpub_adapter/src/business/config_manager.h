#ifndef CFFEX_MDBPUB_CONFIG_MANAGER_H
#define CFFEX_MDBPUB_CONFIG_MANAGER_H

#include <cffex/singleton.h>
#include <cffex/common/xml_config_parser.h>
#include <cffex/log/loghelper.h>

const int g_max_buf_len = 1024;

class config_manager : public cffex::singleton<config_manager>
{
public:
    void parse_config_file(const char *file_name)
    {
        if(parser_.parse_file(file_name) < 0)
        {
            XLOG(XLOG_ERROR, "config_manager::%s error_msg[%s]\n", __FUNCTION__, parser_.get_error_message().c_str());
        }

        memset(mdbpub_addr_, 0, sizeof(mdbpub_addr_) * sizeof(char));
        memset(flowpath_, 0, sizeof(flowpath_) * sizeof(char));
        memset(xtp_pub_addr_, 0, sizeof(xtp_pub_addr_) * sizeof(char));
        memset(logfile_, 0, sizeof(logfile_) * sizeof(char));
        memset(loglevel_, 0, sizeof(loglevel_) * sizeof(char));

        strncpy(mdbpub_addr_, parser_.get_parameter("mdbpub/MdbpubAddr").c_str(), sizeof(mdbpub_addr_));
        strncpy(flowpath_, parser_.get_parameter("mdbpub/FlowPath").c_str(), sizeof(flowpath_));
        strncpy(xtp_pub_addr_, parser_.get_parameter("publish/XTPPubAddr").c_str(), sizeof(xtp_pub_addr_));
        strncpy(logfile_, parser_.get_parameter("log/FilePath").c_str(), sizeof(logfile_));
        strncpy(loglevel_, parser_.get_parameter("log/LogLevel").c_str(), sizeof(loglevel_));
    }

    const char *get_mdbpub_addr()
    {
        return mdbpub_addr_;
    }

    const char *get_flow_path()
    {
        return flowpath_;
    }

    uint32_t get_mdbpub_topic()
    {
        return get_hex_int("mdbpub/MdbpubTopic");
    }

    const char *get_xtp_pub_addr()
    {
        return xtp_pub_addr_;
    }

    uint32_t get_xtp_pub_topic()
    {
        return get_hex_int("publish/XTPPubTopic");
    }

    int get_xtp_pub_threadnum()
    {
        return get_integer("publish/XTPPubThreadNum");
    }

    const char *get_log_file_path()
    {
        return logfile_;
    }

    const char *get_log_level()
    {
        return loglevel_;
    }

private:
    const char *get_string(const char *key)
    {
        std::string res = parser_.get_parameter(key);
        if(res.empty())
        {
            XLOG(XLOG_ERROR, "config_manager::%s get config[%s] failed!\n", __FUNCTION__, key);
            exit(-1);
        }
        return res.c_str();
    }

    uint32_t get_hex_int(const char *key)
    {
        std::string res = parser_.get_parameter(key);
        if(res.empty())
        {
            XLOG(XLOG_ERROR, "config_manager::%s get config[%s] failed!\n", __FUNCTION__, key);
            exit(-1);
        }

        uint32_t ret;
        sscanf(res.c_str(), "%x", &ret);
        return ret;
    }

    int get_integer(const char *key)
    {
        std::string res = parser_.get_parameter(key);
        if(res.empty())
        {
            XLOG(XLOG_ERROR, "config_manager::%s get config[%s] failed!\n", __FUNCTION__, key);
            exit(-1);
        }

        return atoi(res.c_str());
    }

private:
    cffex::common::xml_config_parser parser_;

    char mdbpub_addr_[g_max_buf_len];
    char flowpath_[g_max_buf_len];
    char xtp_pub_addr_[g_max_buf_len];
    char logfile_[g_max_buf_len];
    char loglevel_[g_max_buf_len];
};


#endif