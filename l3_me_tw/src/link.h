/********************************************//**
 * \file	link.h
 * \brief	Definition of link.
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef LINK_H
#define LINK_H
////////////////////////////////////////
class Device;
class Computer;
class Router;
//========================================
/*! \brief Link
 *
 * Class for link implementation.
 *
 */
class Link
{
private:
	double busy[2];							//!< Sum of times, when the link was busy.
	int id;									//!< ID of link.
	Message *msg[2];						//!< Pointer to message actually passing through the link.
											// msg[0] <=> pass 0 -> 1, msg[0] <=> pass 1 -> 0
public:
	static const int capacity = 102400;		//!< Capacity of link  (100 Mb/s = 102400 Kb/s).
	Device *devices[2];						//!< Pointers to devices plugged with link.

	//! Constructor.
	Link(int idr);

	//! Checks availability of selected way in link.
	bool check_availability(int i) { return (msg[i] == 0) ? true : false; }

	//! Adds message to link.
	void add_msg_to_link(int which_output, int link_nr);

	//! Sends message to selected plugged device.
	bool send_msg_to_device(int i);

	//! Getter for pointer of device.
	Device *get_device(int i);

	//! Getter for message.
	Message *get_msg(int i) { return msg[i]; }

	//! Getter for message size.
	int get_size(int i) { return msg[i]->get_size(); }

	//! Getter of busy times.
	void get_busy(double bs[2]) { bs[0] = busy[0]; bs[1] = busy[1]; }

	//! Reset statistics.
	void reset_statistics();

	//! Delete messages.
	void delete_msgs();

	//! Friend function to connect links with routers.
	friend bool connect(Link &link, Router &router, int link_it, int dev_it);

	//! Friend function to connect links with computers.
	friend bool connect(Link &link, Computer &computer);
};

#endif /* LINK_H */