/********************************************//**
 * \file   link.cpp
 * \brief  Link definition.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-04-26
 ***********************************************/
#include "sc.h"
//================================================
Link::Link(int idr) : id(idr)
{
	devices[0] = 0;
	devices[1] = 0;
	msg[0] = 0;				// passing device 0 -> 1
	msg[1] = 0;				// passing device 1 -> 0
	busy[0] = 0;			// same as in msg[]
	busy[1] = 0;
}
//====================================================
void Link::add_msg_to_link(int which_output, int link_nr)
{
	if(msg[which_output] == 0)
	{
		msg[which_output] = devices[which_output]->get_and_del_msg(link_nr);
	}
}
//====================================================
bool Link::send_msg_to_device(int which_msg)
{
	if(msg[which_msg] != 0)
	{
		busy[which_msg] += (double) msg[which_msg]->get_size() / capacity;	// Update time of being busy
		devices[(which_msg == 0) ? 1 : 0]->save_msg(msg[which_msg]);
		msg[which_msg] = 0;
		return true;
	}
	return false;
}
//====================================================
Device *Link::get_device(int i)
{
	if(i < 2 && i >= 0)
		return devices[i];
	return 0;
}
//====================================================
void Link::reset_statistics()
{
	busy[0] = 0;
	busy[1] = 0;
}
//====================================================
void Link::delete_msgs()
{
	if(msg[0]) {delete msg[0];msg[0] = 0;}
	if(msg[1]) {delete msg[1];msg[1] = 0;}
}
//====================================================
bool connect(Link &link, Computer &computer)
{
	if(link.devices[0] == 0 && computer.link == 0)				// checking if devices are not
	{															// already connected
		link.devices[0] = &computer;
		computer.link = &link;
		return true;
	}
	return false;
}
//====================================================
bool connect(Link &link, Router &router, int link_it, int dev_it)
{
	if(link.devices[link_it] == 0 && router.links[dev_it] == 0) // checking if devices are not
	{															// already connected
		link.devices[link_it] = &router;
		router.links[dev_it] = &link;
		return true;
	}
	return false;
}