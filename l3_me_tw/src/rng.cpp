/********************************************//**
 * \file   rng.cpp
 * \brief  Random number generators with exponential and normal distributions.
 * \author Krzysztof Bakowski (krzysztof.bakowski@put.poznan.pl)
 * \date   2013-03-21
 ***********************************************/
#include "sc.h"
/////////////////////////////////////////////////
const int q = 127773;
const int r = 2836;
const int range = 2147483647; //2^31-1
//-----------------------------------------------
double Uniform(void)
{  
	static int x=3566;

	int h = int(x / q);	
	
	x = 16807 * (x - q * h) - r * h;

	if(x < 0) x += range;

	return static_cast<long double>(x) / static_cast<long double>(range);
}
//-----------------------------------------------
double Uniform(int& x)
{	
	register int h = int(x / q);

	x = 16807 * (x - q * h) - r * h;
	
	if (x < 0) x += range;

	return static_cast<long double>(x) / static_cast<long double>(range);
}
/////////////////////////////////////////////////
//***********************************************
