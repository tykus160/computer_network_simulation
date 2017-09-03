/********************************************//**
 * \file   rng.cpp
 * \brief  Random number generators with exponential and normal distributions.
 * \author Krzysztof Bakowski (krzysztof.bakowski@put.poznan.pl)
 * \date   2013-03-21
 ***********************************************/
#ifndef RNG_H
#define RNG_H
//-----------------------------------------------
extern  const int q;
extern  const int r;
extern  const int range;
//-----------------------------------------------
double Uniform(void);	//!< Uniform_distribution from 0 to 1 with internal seed.
double Uniform(int& x); //!< Uniform_distribution from 0 to 1 with provided seed variable.
//----------------------------------------------
inline double Exponential(const double & lambda)
{
	return -log(Uniform()) * lambda;
}
//---------------------------------------------
inline double Exponential(double& lambda, int& x)
{
	return -log(Uniform(x)) * lambda;
}
//-----------------------------------------------
inline double Normal()
{
	const int n=24;								// mi = n/2, sigma = sqrt(n / 12)

	double X=0;
	
	for(int i = 0; i < n; i++) X += Uniform();	// X -= double(n) / 2.0;
	
	return X;									// X / sqrt(double(n) / 12.0);
}
/////////////////////////////////////////////////
//***********************************************
#endif /*RNG_H*/
