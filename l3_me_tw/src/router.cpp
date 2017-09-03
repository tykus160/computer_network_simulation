/********************************************//**
 * \file   router.cpp
 * \brief  Router definition.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-04-26
 ***********************************************/
#include "sc.h"
//================================================
Router::Router(int id, int devices, int routing[]) : Device(id, BR, devices)
{
	links = new Link*[devices];
	for(int i = 0; i < devices; ++i)
	{
		links[i] = 0;
	}
	for(int i = 0; i <NR_OF_PCS; ++i)
	{
		this->routing[i] = routing[i];
	}
}
//================================================
Router::~Router()
{
	delete [] links;
}
//================================================
bool Router::send_msg(int which)
{
	int j = (this == links[which]->get_device(0)) ? 0 : 1;
	if(links[which]->check_availability(j))
	{
		links[which]->add_msg_to_link(j, which);
		return true;
	}
	return false;
}
//================================================
bool Router::save_msg(Message *msg)
{
	int output = routing[msg->get_destination()];
	return buffer.add_to_queue(msg, output);
}