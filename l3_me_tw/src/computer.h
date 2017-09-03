/********************************************//**
 * \file	computer.h
 * \brief	Computer object definition.
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef COMPUTER_H
#define COMPUTER_H
////////////////////////////////////////
/*! \brief Computer
 *
 * Computer is place, where messages are being created.
 * It is also a final receiver of packets.
 * This class inherits from Device.
 *
 */
class Computer : public Device
{
public:
	Link *link;	//!< Pointer to link.

	//! Constructor.
	Computer(int id);

	//! Send message to another device.
	bool send_msg();

	//! Function to inform about successful (or not) message delivery.
	bool save_msg(Message *msg);

	//! Saves message after creation.
	bool save_msg_to_buffer(Message *msg) { return buffer.add_to_queue(msg, 0); }

	//! Checks if link is busy (false) or not (true).
	bool check_link() { return (link->get_msg(0) == 0) ? true : false; }

	//! Friend function to connect links with computers.
	friend bool connect(Link &link, Computer &computer);
};

#endif /* COMPUTER_H */