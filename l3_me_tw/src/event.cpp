/********************************************//**
 * \file   event.cpp
 * \brief  Time events.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-05-24
 ***********************************************/
#include "sc.h"
//===========================================
void GenerateMessage::execute()
{
	agenda.schedule(new GenerateMessage(pc, lambda, priority, seed));	// plan next time event
	if(!priority)				// P1 message
	{
		int size = (int) (Uniform() * 100 + 100);	// size = <100, 200>
		Hub *hub = new Hub(size, pc->get_id(), event_time);

		// Conditional event
		for(int i = 0; i < 4; ++i)
		{
			pc->save_msg_to_buffer(hub->messages[i]);
		}
	}
	else						// P2 message
	{
		int size = (int) (Uniform() * 500 + 500);	// size = <500, 1000>
		int dest;
		do { dest = (int) (Uniform() * 4); } while(dest == pc->get_id());
		Message *msg = new Message(priority, size, event_time, dest, pc->get_id());
		
		// Conditional event
		pc->save_msg_to_buffer(msg);
	}
}
//===========================================
void EndTransmission::execute()
{
	link->get_device(end)->get_buffer()->decrement_size(link->get_size(end));
	link->send_msg_to_device(end);
}