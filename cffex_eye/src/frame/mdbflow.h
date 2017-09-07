#ifndef CFFEX_MDBFLOW_H
#define CFFEX_MDBFLOW_H

#include <cffex/log/loghelper.h>
#include <cffex/file.h>

class mdb_user_flow
{
public:
    mdb_user_flow(const char *filepath, const char *filename)
    {
        char flowname[1024] = {0};
        snprintf(flowname, sizeof(flowname)*sizeof(char), "%s%s.con", filepath, filename);

        trading_day_.clear();
        count_ = 0;
        if(file_.open(flowname, "rb", false))
        {
            file_.setpos(0);
            uint32_t cnt;
            if(file_.read(trading_day_.get_addr(), trading_day_.size()) != 0
               && file_.read(&cnt, sizeof(cnt)) != 0)
            {
                count_ = g_ntohl(cnt);
                XLOG(XLOG_DEBUG, "=======mdb_user_flow::%s trading_day[%s] count[%u]\n",
                    __FUNCTION__, trading_day_.get_value(), count_);
            }
        }

        if(!file_.open(flowname, "wb+", true))
        {
            XLOG(XLOG_ERROR, "mdb_user_flow::%s failed to open file[%s]\n",
                __FUNCTION__, flowname);
            throw std::runtime_error("failed to open file");
        }
        write_file();
    }
    ~mdb_user_flow()
    {
        file_.close();
    }

    uint32_t get_count()
    {
        return count_;
    }

    void set_count(uint32_t count)
    {
        if(count_ != count)
        {
            count_ = count;
            write_file();
        }
    }

    const char *get_trading_day()
    {
        return trading_day_.get_value();
    }

    void set_trading_day(const char *trading_day)
    {
        if(trading_day_ != trading_day)
        {
            trading_day_ = trading_day;
            count_ = 0;
            write_file();
        }
    }

private:
    void write_file()
    {
        file_.setpos(0);
        file_.write(trading_day_.get_value(), trading_day_.size());
        uint32_t cnt = g_htonl(count_);
        file_.write(&cnt, sizeof(cnt));
        file_.flush();
    }

private:
    cffex::ptrade::CDateType trading_day_;
    uint32_t count_;
    cffex::file file_;
};

#endif