/********************************************//**
 * \file	buffer.h
 * \brief	Buffer object definition.
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef BUFFER_H
#define BUFFER_H
////////////////////////////////////////
/*! \brief Buffer
 *
 * Buffer is place, where messages are stored.
 *
 */
class Buffer
{
private:
	int devices;				//!< Number of lists in buffer (any link has it's buffer).
	int actual_size;			//!< Actual size of buffer (in kbit).
	list<Message*> *msg_list;	//!< List with messages in a buffer.

public:
	const int max_size;			//!< Maximum size of buffer (in kbit).

	//! Constructor.
	Buffer(int size, int devices);

	//! Destructor.
	~Buffer();

	//! Add message to priority queue.
	bool add_to_queue(Message *msg, int which_buff);

	//! Get message and delete it from queue.
	Message *get_and_del_msg(int i);

	//! Get message.
	Message *get_msg(int i) { return msg_list[i].front(); }

	//! Get ID of message at the top of queue.
	int get_top_destination(int i);

	//! Decrement size of buffer.
	void decrement_size(int size) { actual_size -= size; }

	//! Check if msg_list is empty (true if yes).
	bool check_msg_list(int i) { return msg_list[i].empty(); }

	//! Get actual size.
	int get_actual_size() { return actual_size; }

	//! Reset buffer.
	void reset_buffer();
};

#endif /* BUFFER_H */