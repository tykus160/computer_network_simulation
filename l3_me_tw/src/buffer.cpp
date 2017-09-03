/********************************************//**
 * \file   buffer.cpp
 * \brief  Buffer definition.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-04-26
 ***********************************************/
#include "sc.h"
//================================================
Buffer::Buffer(int size, int nr) : max_size(size), actual_size(0), devices(nr)
{
	msg_list = new list<Message*>[devices];
}
//================================================
Buffer::~Buffer()
{
	for(int i = 0; i < devices; ++i)
	{
		list<Message*>::iterator j;
		for(j = msg_list[i].begin(); j != msg_list[i].end(); ++j)
			delete (*j);
		msg_list[i].clear();
	}
	delete [] msg_list;
}
//================================================
bool Buffer::add_to_queue(Message *msg, int which_buff)
{
	if(msg->get_size() <= max_size - actual_size)
	{
		actual_size += msg->get_size();
		if(msg->get_priority())
		{
			msg_list[which_buff].push_back(msg);
		}
		else
		{
			if(!msg_list[which_buff].empty())
			{
				list<Message*>::iterator it = msg_list[which_buff].begin();
				while(it != msg_list[which_buff].end() && !((*it)->get_priority()) ) ++it;
					msg_list[which_buff].insert(it, msg);
			}
			else
			{
				msg_list[which_buff].push_front(msg);
			}
			
		}
		return true;
	}
	else
	{
		if(msg->get_priority())Message::incr_lost_msgs();
		else msg->get_parent()->incr_lost_msgs();
		return false;
	}
}
//================================================
Message *Buffer::get_and_del_msg(int i)
{
	if(!msg_list[i].empty())
	{
		Message *msg = msg_list[i].front();	// Get message.
		msg_list[i].pop_front();			// Delete message from buffer.
		return msg;
	}
	return 0;
}
//================================================
int Buffer::get_top_destination(int i)
{
	Message *msg = msg_list[i].front();
	return msg->get_destination();
}
//================================================
void Buffer::reset_buffer()
{
	actual_size = 0;

	for(int i = 0; i < devices; ++i)
	{
		msg_list[i].clear();
	}
}
//================================================