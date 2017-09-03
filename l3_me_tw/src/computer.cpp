/********************************************//**
 * \file   computer.cpp
 * \brief  Computer definition.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-04-26
 ***********************************************/
#include "sc.h"
//================================================
Computer::Computer(int id) : Device(id, BK), link(0) { }
//================================================
bool Computer::send_msg()
{
	if(link->check_availability(0))
	{
		link->add_msg_to_link(0,0);
		return true;
	}
	return false;
}
//================================================
bool Computer::save_msg(Message *msg)
{
	//msg->deliver_message();
	//cout << "Computer " << get_id() << " has got the message!" << endl;
	if(msg->deliver_message()) delete msg;
	return true;
}
//================================================