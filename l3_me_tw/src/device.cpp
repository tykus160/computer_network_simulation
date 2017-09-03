/********************************************//**
 * \file   device.cpp
 * \brief  Device definition.
 * \author Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date   2014-04-26
 ***********************************************/
#include "sc.h"
//================================================
Device::Device(int desc, int size, int devices) : buffer(size, devices), id(desc), nr_of_devices(devices)
{
}
//================================================
//void Device::log_to_file()
//{
//	if(!want) return;
//	string device;
//	if(buffer.max_size == BK)
//		device = "Computer";
//	else device = "Router";
//	buffers << "[" << sim_clock << "] "
//		<< device << " " << id << " size: "
//		<< buffer.get_actual_size() << endl;
//}