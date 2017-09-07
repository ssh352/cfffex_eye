#include <gtest/gtest.h>
#include "so_service.hpp"
#include <vector>

class plugin_1 : public iplugin
{
    virtual std::vector<int> get_service_id() {
        std::vector<int> v;
        v.push_back(100);
        return v;
    }
	virtual int init_so(iplugin_host *host) { return 0; };
	virtual void regist_subscribe_topic(std::vector<uint16_t> &sub_topics) {}
	virtual int assign_task(uint16_t fid, char const *data) {return 0;}
	virtual int assign_request(request_info *req_info) {return 0;}
};

TEST(so_service, c1)
{
    plugin_1 *pin = new plugin_1();
    so_plugin *so_p = new so_plugin();
    so_p->set_plugin(pin);

    so_service s;
    s.add_service(so_p);

    EXPECT_EQ(NULL, s.get_service_plugin(200));
    EXPECT_EQ(so_p, s.get_service_plugin(100));

}