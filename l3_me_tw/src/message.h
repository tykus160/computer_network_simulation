/********************************************//**
 * \file	message.h
 * \brief	Message object definition.
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef MESSAGE_H
#define MESSAGE_H
////////////////////////////////////////
class Hub;
////////////////////////////////////////
/*! \brief Message
 *
 * Class for message implementation.
 *
 */
class Message
{
private:
	bool priority;				//!< Describes priority of message (0 - higher priority).
	int size;					//!< Size of message (in kbit).
	double time_created;		//!< Time, when message was created.
	double time_delivered;		//!< Time, when message was delivered (0 if still not).
	int destination;			//!< ID of destination device.
	int creation;				//!< ID of place, where message was created.
	Hub *parent;				//!< Pointer to hub for P1 messages.

	static int msgs_created[2];		//!< Counter of created messages (index 0 - P1, 1 - P2).
	static int msgs_delivered[2];	//!< Counter of delivered messages.
	static int msgs_lost[2];		//!< Counter of lost messages.
	static double max_delay;		//!< The greatest time delay.
	static double delays[2];		//!< Sum of delays.

public:
	friend class Hub;

	//! Constructor.
	Message(bool pr, int s, double time, int dest, int cr, Hub *par = 0);

	//! Destructor.
	~Message();

	//! Getter of message size.
	int get_size() { return size; }

	//! Getter of message ID.
	int get_destination() { return destination; }

	//! Getter of priority.
	bool get_priority() { return priority; }

	//! Function for message delivery.
	bool deliver_message();

	//! Getter of parent.
	Hub *get_parent() { return parent; }

	//! Increment lost messages.
	static void incr_lost_msgs() { ++msgs_lost[1]; }

	//! Getter of created messages counter.
	static void get_msgs_created(int msgs[2]) { msgs[0] = msgs_created[0]; msgs[1] = msgs_created[1]; }

	//! Getter of delivered messages counter.
	static void get_msgs_delivered(int msgs[2]) { msgs[0] = msgs_delivered[0]; msgs[1] = msgs_delivered[1]; }

	//! Getter of lost messages counter.
	static void get_msgs_lost(int msgs[2]) { msgs[0] = msgs_lost[0]; msgs[1] = msgs_lost[1]; }

	//! Getter of average delay.
	static double get_average_delay(int i);

	//! Getter of max delay.
	static double get_max_delay() { return max_delay; }

	//! Reset statistics.
	static void reset_statistics();
};
//=====================================================================
/*! \brief Hub
 *
 * Additional class for Message class.
 * It helps in managing P1 messages.
 *
 */
class Hub
{
private:
	//bool delivered[4];		//!< Information about which message is already delivered.
	//bool lost;				//!< Informs if any of messages has lost.
	int delivered;
	int lost;

public:
	Message *messages[4];	//!< Pointers to P1 messages created in the same time.

	//! Constructor.
	Hub(int size, int creator, double time);

	//! Destructor.
	~Hub();

	//! Increment lost messages.
	void incr_lost_msgs();

	friend class Message;
};

#endif /* MESSAGE_H */