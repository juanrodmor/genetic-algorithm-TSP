#ifndef CITY_H
	#define CITY_H
	#include <utility>
	
	
	using namespace std;

/**
*	This struct defines a city in our problem. It represents the city and its position.
*/
struct city
{
	
	city(int node, int coor_x, int coor_y): id(node) {coord = make_pair(coor_x, coor_y);}
	city() {}
	
	const int getNode() const { return id; } 
	const pair<int, int>& getCoord() const { return coord; }
	
	private:
		int id;
		pair<int,int> coord;
};

inline operator <(const city& c1, const city& c2)
{
	return (c1.getNode() < c2.getNode());
}



#endif