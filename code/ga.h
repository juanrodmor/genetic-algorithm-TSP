#ifndef GA_H
	#define GA_H
	#include "city.h"
	#include <map>
	
/**
*	This function defines the behavior of the algorithm.
*	
*	P_cross - probability of cross two individuals
*	P_mutation - probability of mutate an individual
*	ciudades - cities of our problem
*	numGenerations - population to be generated (iterations of algorithm)
*	sizePob - size of population (number of individuals)	
*
*/
void GA_main(double P_cross, double P_mutation, map<int, city>& ciudades, int numGenerations, int sizePob);




#endif