#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <stdlib.h> 
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <chrono>
#include "city.h"
#include "ga.h"


using namespace std;
typedef map<int, vector<int>> popStruct;

bool operator <(const pair<int, double>& p1, pair<int, double>& p2)
{
	return p1.second < p2.second;
}

/**
*	initPopulation - Init a population randomly
*
*	population - struct which contains all the individuals
*	sizePob - size of population to generate
*	numCities - number of cities for the instance problem
*
*/
void initPopulation(popStruct& population, int sizePob, int numCities)
{
	srand (unsigned(time(0))); //seed for rand generator
	vector<int> candidates;
	//initialize vector 1...N
	for(int i = 1 ; i <= numCities ; i++)
		candidates.push_back(i);

	for(int i = 0 ; i < sizePob ; i++) //generating sizePob individuals randomly
	{
		random_shuffle(candidates.begin(), candidates.end()); 
		population[i] = candidates;
	}
}

/**
*	Calculate the euclidean distance 2D between two cities.
*
*/
double d_euclid(city& c1, city& c2)
{
	int x1 = c1.getCoord().first; int y1 = c1.getCoord().second;
	int x2 = c2.getCoord().first; int y2 = c2.getCoord().second;

	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

/**
*	Returns the distance of a given path
*
*	individual - index of cities in the path
*	cities - set of city of our problem
*/
double getDistance(vector<int>& individual, map<int, city>& cities)
{
	double d = 0;
	for(int i = 0 ; i < individual.size() - 1; i++)
	{
		d += d_euclid(cities[individual[i]], cities[individual[i+1]]);
	}
	d += d_euclid(cities[individual[individual.size()-1]], cities[individual[0]]); //adding distance from last city to the initial city
	return d;
}

/**
* Constructs a map<int, double> with the distance of each path for every individual.
*/
void evaluatePopulation(popStruct& population, map<int, city>& cities, map<int, double>& evaluation)
{
	for(int i = 0 ; i < population.size() ; i++)
	{
		evaluation[i] = getDistance(population[i], cities);	// Obtain path distance
	}
}

/**
*
*	Selects a invidual from population by tournalement selection method
*
*		k - individuals to be selected randomly	
*
*/
pair<int, vector<int>> tournamentSelection(popStruct& population, map<int, double>& evaluation, int k)
{
	pair<int, vector<int>> best;
	double bestPuntuation = numeric_limits<double>::infinity();
	
	for(int i = 0 ; i < k ; i++)
	{
		int ind = rand() % evaluation.size();	// selecting a random index --> random invidual
		if(evaluation[ind] < bestPuntuation)	// checking if individual selected is better than the previous one
		{
			bestPuntuation = evaluation[ind];
			best = make_pair(ind, population[ind]);
		}
	}
	
	return best;
	
}

/**
*	This function mutates a individual by taking two random genes (positions) with a probability P_mutation
*
*	
*/
vector<int> mutate(const vector<int>& children, double P_mutation)
{
	vector<int> childrenMutated(children);
	int randomIndex_1 = 0;
	int randomIndex_2 = 0;
	
	while(randomIndex_1 == randomIndex_2)	// generating two random index where second must be greater than the first
	{
		randomIndex_1 = rand() % children.size();
		randomIndex_2 = rand() % children.size();
	}
	
	// exchanging genes values 
	int aux = childrenMutated[randomIndex_1];
	childrenMutated[randomIndex_1] = children[randomIndex_2];
	childrenMutated[randomIndex_2] = aux;
	
	return childrenMutated;
}

/**
*	Returns true if a vector v contains a element e. Returns false otherwise.
*
*/
bool hasElement(int e, vector<int> v)
{
	int n = 0; 
	bool follow = true;
	while(n < v.size() && follow)
	{
		if(e == v[n])
			follow = false;
		n++;
	}
	return !follow;
}

/**
*	PMX cross to obtain a new individual from two parents with a probability P_cross
*
*/
map<int, vector<int>> PMX_cross(vector<int>& p1, vector<int>& p2, double P_cross)
{
	typedef std::chrono::high_resolution_clock myclock; 	// set clock
	myclock::time_point beginning = myclock::now();
	
	uniform_real_distribution<double> distribution(0.0,1.0);
	
	myclock::duration d = myclock::now() - beginning;
	unsigned seed = d.count();	// generates a new seed that is equal to system time
	minstd_rand0 generator (seed);
	
	map<int, vector<int>> childrens;
	uniform_int_distribution<int> distInt(1,(p1.size()-1)/2);
	int cut1 = distInt(generator);	
	int cut2 = distInt(generator);
	
	while(cut1 >= cut2)		// generating two random cuts where cut2 must be greater than cut1
	{
		cut1 = distInt(generator);
		cut2 = distInt(generator);
	}
	
	// initializing two childrens with all genes equal to -1
	vector<int> children_1(p1.size(), -1);
	vector<int> children_2(p1.size(), -1);
	
	double probability = distribution(generator); // Generating a random probability
	
	if(probability <= P_cross && p1.size() > 2 && p2.size() > 2)
	{
		// Copying from parents the elements between cut1 and cut2
		for(int i = cut1+1 ; i < cut2 ; i++)
		{
			children_1[i] = p2[i];
			children_2[i] = p1[i];
		}

		// copying elements before cut1 that are not repeated
		for(int i = cut1; i >= 0 ; i--)
		{
			if(!hasElement(p1[i], children_1))
			{
				children_1[i] = p1[i];
			}	
			
			if(!hasElement(p2[i], children_2))
			{
				children_2[i] = p2[i];
			}			
		}
		
		// copying elements after cut2 until end that are not repeated
		for(int i = cut2; i < p1.size() ; i++)
		{
			if(!hasElement(p1[i], children_1))
			{
				children_1[i] = p1[i];
			}
	
			if(!hasElement(p2[i], children_2))
			{
				children_2[i] = p2[i];
			}			
			
		}		
		
		// Checking -1 values and sustitute them for values not used yet
		for(int i = 0 ; i < children_1.size() ; i++)
		{
			//this counters are used as index in parents
			int cont1 = cut1+1;
			int cont2 = cut1+1;
			
			while(children_1[i] == -1)
			{
				if(!hasElement(p1[cont1], children_1))
					children_1[i] = p1[cont1];
				cont1++;
			}
			
			while(children_2[i] == -1)
			{
				if(!hasElement(p2[cont2], children_2))
					children_2[i] = p2[cont2];
				cont2++;
			}
		}
		
		childrens[0] = children_1;
		childrens[1] = children_2;
		
	}
	else

	
	return childrens;
	
}

/**
*	OX cross to obtain a new individual from two parents with a probability P_cross
*
*/
map<int, vector<int>> OX_cross(vector<int>& p1, vector<int>& p2, double P_cross)
{
	typedef std::chrono::high_resolution_clock myclock; 	// set clock
	myclock::time_point beginning = myclock::now();
	
	uniform_real_distribution<double> distribution(0.0,1.0);
	
	myclock::duration d = myclock::now() - beginning;
	unsigned seed = d.count();	// generates a new seed that is equal to system time
	minstd_rand0 generator (seed);
	
	map<int, vector<int>> childrens;
	uniform_int_distribution<int> distInt(1,(p1.size()-1)/2);
	int cut1 = distInt(generator);	
	int cut2 = distInt(generator);
	
	while(cut1 >= cut2)	// generating two random cuts where cut2 must be greater than cut1
	{
		cut1 = distInt(generator);
		cut2 = distInt(generator);
	}
	
	vector<int> children_1(p1.size(), -1);
	vector<int> children_2(p1.size(), -1);
	
	double probability = distribution(generator); // Generating a random probability
	
	if(probability <= P_cross && p1.size() > 2 && p2.size() > 2)
	{
		// Copying from parents the elements between cut1 and cut2
		for(int i = cut1+1 ; i < cut2 ; i++)
		{
			children_1[i] = p1[i];
			children_2[i] = p2[i];
		}
		
		int cont1 = cut2;
		int cont2 = cut2;
		
		// Starting from second cut until end, copying elements from other parents which is not repeated
		for(int i = cut2 ; i < children_1.size() ; i++)
		{
			while(children_1[i] == -1)
			{
				if(!hasElement(p2[cont1], children_1))
					children_1[i] = p2[cont1];
				cont1++;
				cont1 = cont1 % children_1.size();	// we can expressed the index in parents as a value between 0..size-1
			}
			
			while(children_2[i] == -1)
			{
				if(!hasElement(p1[cont2], children_2))
					children_2[i] = p1[cont2];
				cont2++;
				cont2 = cont2 % children_2.size();
			}
		}
		
		// Starting from begin until first cut, copying elements from other parents which is not repeated
		cont1 = 0; 
		cont2 = 0;
		for(int i = 0 ; i <= cut1 ; i++)
		{
			while(children_1[i] == -1)
			{
				if(!hasElement(p2[cont1], children_1))
					children_1[i] = p2[cont1];
				cont1++;
			}
			
			while(children_2[i] == -1)
			{
				if(!hasElement(p1[cont2], children_2))
					children_2[i] = p1[cont2];
				cont2++;
			}
		}
	
		childrens[0] = children_1;
		childrens[1] = children_2;
	}

	return childrens;
}

/**
*	Gets the best individual from population. Returns the individual and its evaluation
*
*/
pair<vector<int>, double> getBest(map<int,double>& evaluation, popStruct& population)
{
	
	double bestPunt = numeric_limits<double>::infinity();
	pair<vector<int>, double> best;
	
	for(int i = 0 ; i < population.size() ; i++)
	{
		if(evaluation[i] < bestPunt)
		{
			bestPunt = evaluation[i];
			best = make_pair(population[i], bestPunt);
		}
	}
	
	return best;
}	

/**
*	Gets the worst individual from population. Returns the index of the individual.
*	
*/
int getWorse(map<int, double>& evaluation)
{
	double worsePunt = 0;
	int worseInd = -1;
	
	for(int i = 0 ; i < evaluation.size() ; i++)
	{
		if(evaluation[i] > worsePunt)
		{
			worsePunt = evaluation[i];
			worseInd = i;
		}
	}
	return worseInd;
}

/**
*	Replaces the worst individual with the new children obtained after mutation
*
*/
void replace(popStruct& population, map<int, double>& evaluation, popStruct& mutated, map<int, city>& ciudades)
{
	
	map<int, double> evalMutated;
	evaluatePopulation(mutated, ciudades, evalMutated);	// Evaluting the childrens mutated
	
	for(int i = 0 ; i < evalMutated.size() ; i++)
	{
		int worse = getWorse(evaluation);	// getting the worst individual

		if(worse != -1 && evalMutated[i] < evaluation[worse]) // replace it if the new candidate is better
		{
			map<int, double>::iterator itEval = evaluation.find(worse);
			evaluation.erase(itEval);										// Erasing the worst individual
			evaluation[worse] = evalMutated[i];								// Introducing the new
			popStruct::iterator itPop = population.find(worse);
			population.erase(itPop);										// Erasing the worst individual
			population[worse] = mutated[i];									// Introducing the new
		}
	}
	
}

/**
*	Prints a path of cities
*	
*/
void printPath(vector<int>& path)
{
	for(int i = 0 ; i < path.size() ; i++)
		cout << path[i] << " -> ";
	cout << endl;
}

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
void GA_main(double P_cross, double P_mutation, map<int, city>& ciudades, int numGenerations, int sizePob)
{
	
	popStruct population;
	map<int, double> evaluation;
	pair<vector<int>, double> bestSolution;
	pair<vector<int>, double> antSolution;
	
	initPopulation(population, sizePob, ciudades.size());	// initializing population randomly
	
	evaluatePopulation(population, ciudades, evaluation);	// Evaluting population
	
	bestSolution = getBest(evaluation, population);			// getting the best solution at this moment
	antSolution = bestSolution;
	
	int gen = 0;
	
	while(gen < numGenerations)
	{
		
		// Selecting parents
		pair<int, vector<int>> parent1 = tournamentSelection(population, evaluation, population.size());
		pair<int, vector<int>> parent2 = tournamentSelection(population, evaluation, population.size());
		
		// Crossing its
		map<int, vector<int>> childrens = PMX_cross(parent1.second, parent2.second, P_cross);
		
		// If childrens are not the parents, we mutate and replace in population
		if(childrens.size() != 0) 
		{
			popStruct mutated;
			int i = 0;
			for(int i = 0 ; i < childrens.size() ; i++)
			{
				mutated[i] = mutate(childrens[i], P_mutation);
				i++;
			}
			
			
			replace(population, evaluation, mutated, ciudades);
			
			
			evaluation.clear();
			evaluatePopulation(population, ciudades, evaluation); // Reevaluate de new population
		}
	
		gen++;
		
		pair<vector<int>, double> bestLocal = getBest(evaluation, population);
		if(bestLocal.second < bestSolution.second)
			bestSolution = bestLocal;
		
	}

	cout << "[-->]\t Execution finished." << endl;
	cout << "[-->]\t The best solution found has a value of: " << bestSolution.second << endl;
	cout << "[-->]\t The solution path is: " << endl;
	cout << endl;
	printPath(bestSolution.first);
	
	
}
