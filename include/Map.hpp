#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstring>
#include <vector>

#include <Object.hpp>
#include <Monster.hpp>
#include <Waste.hpp>
#include <Weapon.hpp>

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
		std::vector<Square> pixels;
		string ppmFile;
		int width;
		int height;

		void loadObject(string line);
		void loadMonster(string line);
		void loadMap(string filename);
		Map invert();

		Square getEntrance();

};
