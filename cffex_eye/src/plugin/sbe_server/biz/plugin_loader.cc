#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "sbe_server.h"

#ifdef __cplusplus
extern "C"
{
#endif

void* create(){
    sbe_server *p = new sbe_server();

	return (void*)p;
}

void destroy(void* p){
	delete (sbe_server*)p;
}

#ifdef __cplusplus
}
#endif

#endif