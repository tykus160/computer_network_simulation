/********************************************//**
 * \file   event.h
 * \brief  Time event.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-05-24
 ***********************************************/
#ifndef EVENT_H
#define EVENT_H
////////////////////////////////////////////
class GenerateMessage : public Event
{
private:
	Computer *pc;	//!< Pointer to computer.
	double lambda;
	int seed;
	bool priority;	//!< Priority of message.

public:
	//! Constructor.
	GenerateMessage(Computer *pcd, double l, bool pr, int s) : Event(sim_clock + Exponential(l, s)), pc(pcd), lambda(l), priority(pr)
	{
		seed = s;
	}

	//! Destructor.
	~GenerateMessage()
	{
		pc = 0;
	}

	//! Execute a time event.
	void execute();
};
//==============================================
class EndTransmission : public Event
{
private:
	Link *link;		//!< Pointer to link.
	int end;		//!< Informs about end of link.

public:
	//! Constructor.
	EndTransmission(Link *lnk, int i) : Event(sim_clock + (double) lnk->get_size(i) / Link::capacity), link(lnk), end(i) {}

	//! Destructor.
	~EndTransmission()
	{
		link = 0;
	}

	//! Execute a time event.
	void execute();
};

#endif /* EVENT_H */