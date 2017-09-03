/********************************************//**
 * \file	sc.h
 * \brief	Header file for simulation project.
 * \author	Wojciech Tyczynski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#ifndef SC_H
#define SC_H
////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cmath>
#include <list>
////////////////////////////////////////
using namespace std;
////////////////////////////////////////
#define BK 4096
#define BR 8192
#define NR_OF_ROUTERS 4
#define NR_OF_PCS 5
#define NR_OF_LINKS 10
////////////////////////////////////////
extern double sim_clock;
#include "event_scheduling.h"
extern EventList agenda;
extern ofstream msg2;
////////////////////////////////////////
#include "message.h"
#include "buffer.h"
#include "link.h"
#include "device.h"
#include "computer.h"
#include "router.h"
#include "rng.h"
#include "event.h"

#endif /* SC_H */