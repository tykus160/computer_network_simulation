/********************************************//**
 * \file	router.h
 * \brief	Router object definition.
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef ROUTER_H
#define ROUTER_H
////////////////////////////////////////
/*! \brief Router
 *
 * Router stores messages in buffer and decides where should it send.
 *
 */
class Router : public Device
{
private:
	int routing[5];				//!< Routing table. Index: number of PC, value: output link.

public:
	Link **links;				//!< Pointers to links.

	//! Constructor.
	Router(int id, int devices, int routing[]);

	//! Destructor.
	~Router();

	//! Send message to another device.
	bool send_msg(int which);

	//! Checks if link is busy (false) or not (true).
	bool check_link(int i, int j) { return (links[i]->get_msg(j) == 0) ? true : false; }

	//! nr_of_devices getter.
	int get_nr_of_devices() { return nr_of_devices; }

	//! Saves message in buffer.
	bool save_msg(Message *msg);

	//! Friend function to connect links with routers.
	friend bool connect(Link &link, Router &router, int link_it, int dev_it);
};

#endif /* ROUTER_H */