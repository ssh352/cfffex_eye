#include <gtest/gtest.h>

#include "serv_responser.h"
#include "plugin.h"
#include "so_service.hpp"
#include "so_plugin.hpp"
#include "threadtpl.hpp"
#include <cffex/flow/file_flow.h>

TEST(serv_responser, test_name)
{
	threadtpl<so_plugin> *thrd = new threadtpl<so_plugin>();
	so_plugin* sop = new so_plugin();
	pulgin p;
	cffex::file::remove("./t_file_flow.con");
	cffex::file_flow flow(".", "t_file_flow");
	sop->init_default_constructor(&p, &flow, thrd);

	so_service *services = new so_service();
	services->add_service(sop);
    thrd->push_plugin(sop);
    thrd->start();

    serv_responser::get_instance()->init("tcp://127.0.0.1:31110", services);

    while(1)
	{sleep(3);}

	thrd->stop();
	delete services;
	delete sop;
}