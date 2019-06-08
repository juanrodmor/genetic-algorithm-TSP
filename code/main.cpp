#include <iostream>
#include <utility>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h> 
#include <utility>
#include "city.h"
#include "ga.h"
#include <map>
#include <cmath>
#include "cronometro.h"

using namespace std;


void readInput(map<int, city>& cities, string name)
{
	ifstream file; file.open(name);
	
	string line; 
	for (int i = 1; i <= 6 ; i++)
		getline(file, line);
	
	string word = "";
	while(word.compare("EOF") != 0)
	{
		getline(file,line);
		istringstream iss(line);
		vector<string> v_string;
		while(iss >> word)
		{
			v_string.push_back(word);
		}
		if(v_string[0].compare("EOF") != 0)
			cities[atoi(v_string[0].c_str())] = city(atoi(v_string[0].c_str()), atoi(v_string[1].c_str()), atoi(v_string[2].c_str()));
	}
	
	/*
	// PRINTING COORDS 
	for(vector<city>::iterator it = cities.begin(); it != cities.end() ; it++)
	{
		cout << "Ciudad --> " << it->getNode() << ". Coords: " << it->getCoord().first << ", " << it->getCoord().second << endl;
	}*/
	
}

void readSolution(vector<int>& solution, string name)
{
	ifstream file; file.open(name);
	
	string line; 
	for (int i = 1; i <= 4 ; i++)
		getline(file, line);
	
	string word = "";
	while(word.compare("-1") != 0)
	{
		getline(file,line);
		istringstream iss(line);
		vector<string> v_string;
		while(iss >> word)
		{
			v_string.push_back(word);
		}
		if(v_string[0].compare("-1") != 0 || v_string[0].compare("-1") != 0)
			solution.push_back(atoi(v_string[0].c_str()));
	}
	
}

double d_euclid2(city& c1, city& c2)
{
	int x1 = c1.getCoord().first; int y1 = c1.getCoord().second;
	int x2 = c2.getCoord().first; int y2 = c2.getCoord().second;

	//cout << "Distance from " << c1.getNode() << " to " << c2.getNode() << endl;
	
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double getDistance2(vector<int>& individual, map<int, city>& cities)
{
	double d = 0;
	for(int i = 0 ; i < individual.size() - 1; i++)
	{
		//cout << "Distancia from --> " << individual[i] << " to " << individual[i+1] << endl; 
		d += d_euclid2(cities[individual[i]], cities[individual[i+1]]);
	}
	d += d_euclid2(cities[individual[individual.size()-1]], cities[individual[0]]);
	//cout << "distance: " << d << endl;
	return d;
}

int main(int argc, char** argv)
{
	map<int, city> cities;
	cronometro c;
	
	readInput(cities, argv[1]);
	
	int sizePob = atoi(argv[2]);
	double P_cross = atof(argv[3]);
	double P_mutate = atof(argv[4]);
	int numIter = atoi(argv[5]);

	c.activar();
	GA_main(P_cross, P_mutate, cities, numIter, sizePob);
	c.parar();
		
	if(argv[6] != NULL)
	{
		// Printing distance for best solution in opt.tour file
		vector<int> solution;
		readSolution(solution, "berlin52.opt.tour");
		double dSolution = getDistance2(solution, cities);
		cout << endl;
		cout << "[-->]\t Best solution for the problem: " << dSolution << endl;
	}
	
	cout << "[-->]\t Execution time: " << c.tiempo() << endl;
	
	return 0;
}