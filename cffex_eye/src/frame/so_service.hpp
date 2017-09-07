#ifndef SO_SERVICE_HEAD
#define SO_SERVICE_HEAD

#include "so_plugin.hpp"
#include <cffex/log/loghelper.h>
#include <map>

//将so中的服务组装成map
//提供服务的增加和查询服务
class so_service
{
    typedef std::map<int, so_plugin*> service_map; //<service_id, so*>

public:
    so_service() {};
    ~so_service() {};

    void add_service(so_plugin *plugin)
    {
        std::vector<int> services = plugin->get_service_id();
        for (std::vector<int>::iterator i = services.begin(); i != services.end(); ++i)
        {
            XLOG(XLOG_DEBUG, "so_service::%s, service_id[%d], *so_plugin[%p]\n", __FUNCTION__, *i, plugin);
            service_map_.insert(service_map::value_type(*i, plugin));
        }
    }

    so_plugin* get_service_plugin(int service_id)
    {
        service_map::iterator i = service_map_.find(service_id);
        if(i!=service_map_.end())
        {
            so_plugin* p = i->second;
            XLOG(XLOG_DEBUG, "so_service::%s, service_id[%d], *so_plugin[%p]\n", __FUNCTION__, service_id, p);
            return p;
        }
        else
        {
            XLOG(XLOG_WARNING, "so_service::%s, cann't found service_id[%d]\n", __FUNCTION__, service_id);
            return NULL;
        }
    }

private:
    service_map service_map_;

};

#endif
