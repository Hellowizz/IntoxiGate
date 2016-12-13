#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <Object.hpp>
#include <Monster.hpp>

using namespace std;

enum squareType {wall, hall, door, acid, safeRoom, getIn, getOut};

typedef struct square {
	Position pos;
	squareType type;
} Square;

class Map {
	public:
		string mapName;
		list<Object> objects;
		list<Monster> monsters;
		Square *pixels;
		string ppmFile;

		void loadObject(string line);
		void loadMonster(string line);
		void loadMap(string filename);

};

#endif