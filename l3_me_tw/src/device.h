/********************************************//**
 * \file	device.h
 * \brief	Device definition (basic class for Router and Computer).
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef DEVICE_H
#define DEVICE_H
////////////////////////////////////////
/*! \brief Device
 *
 * Abstract class for device implementation.
 * It implements common attributes and methods for Router and Computer.
 *
 */
class Device
{
private:
	int id;						//!< ID of device.

protected:
	Buffer buffer;				//!< Buffer of device.
	const int nr_of_devices;	//!< Information about how many devices are plugged to the router.

public:
	//! Constructor.
	Device(int desc, int size, int devices = 1);

	//! Get id.
	int get_id() { return id; }

	//! Pure virtual function to save message in buffer.
	virtual bool save_msg(Message *msg) = 0;

	//! Get message from buffer.
	Message *get_msg(int i) { return buffer.get_msg(i); }

	//! Get message from buffer and delete it.
	Message *get_and_del_msg(int i) { return buffer.get_and_del_msg(i); }

	//! Get buffer.
	Buffer *get_buffer() { return &buffer; }

	//! Get number of devices.
	const int get_nr_of_devices() { return nr_of_devices; }
};

#endif /* DEVICE_H */