/********************************************//**
 * \file   message.cpp
 * \brief  Message definition.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-04-26
 ***********************************************/
#include "sc.h"
//================================================
int Message::msgs_created[2] =		{0};
int Message::msgs_delivered[2] =	{0};
int Message::msgs_lost[2] =			{0};
double Message::max_delay =			0;
double Message::delays[2] =			{0};
//================================================
Message::Message(bool pr, int s, double time, int dest, int cr, Hub *par) : 
	priority(pr), size(s), time_created(time), destination(dest),
	creation(cr), time_delivered(0), parent(par)
{
	if(pr == 1) ++msgs_created[1];
}
//================================================
Message::~Message()
{
	parent = 0;
}
//================================================
bool Message::deliver_message()
{
	bool check = false;
	time_delivered = sim_clock;
	double del = time_delivered - time_created;
	if(priority == 1)
	{
		++msgs_delivered[1];
		delays[1] += del;
		if(msgs_delivered[1] < 1000) msg2 << sim_clock << " " << del << endl;
		check = true;
	}
	else if(++parent->delivered == 4)
	{ // We do not add delays from all copies, only the greatest one.
		del = time_delivered - time_created;
		++msgs_delivered[0];
		double temp = 0.0;
		for(int i = 0; i < 4; ++i)
		{
			temp = parent->messages[i]->time_delivered - parent->messages[i]->time_created;
			if(temp > del) del = temp;	// Searching for the greatest time delay.
		}
		delays[0] += del;
		//msg1 << sim_clock << " " << del << endl;
		check = true;
	}
	if(del > max_delay) max_delay = del;
	return check;
}
//================================================
double Message::get_average_delay(int i)
{
	//int all = msgs_delivered[i] + msgs_delivered[i];
	return delays[i] / msgs_delivered[i];
}
//================================================
void Message::reset_statistics()
{
	msgs_created[0] = 0;
	msgs_created[1] = 0;
	msgs_delivered[0] = 0;
	msgs_delivered[1] = 0;
	msgs_lost[0] = 0;
	msgs_lost[1] = 0;
	max_delay = 0;
	delays[0] = 0;
	delays[1] = 0;
}
//================================================
Hub::Hub(int size, int creator, double time)
{
	++Message::msgs_created[0];
	lost = 0;
	delivered = 0;
	int i = 0, j = 0;
	while(i < 4)
	{
		if(j == creator) ++j;
		messages[i++] = new Message(0, size, time, j++, creator, this);
	}
}
//================================================
Hub::~Hub()
{
	for(int i = 0; i < 4; ++i) delete messages[i];
}
//================================================
void Hub::incr_lost_msgs()
{
	if(!lost)
	{
		++Message::msgs_lost[0];
	}
	++lost;
}