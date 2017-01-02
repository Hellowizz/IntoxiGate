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
#include <Key.hpp>

enum squareType {wall, hall, door, acid, safeRoom, getIn, getOut};

typedef struct square {
	Position pos;
	squareType type;
} Square;

class Map {
	public:
		string mapName;
		vector<Object> objects;
		vector<Monster> monsters;
		vector<Square> pixels;
		string ppmFile;
		int width;
		int height;

		Map();

		void loadObject(string line);
		void loadMonster(string line);
		void loadMap(string filename);
		Map invert();

		Square getEntrance();
		int isObject(float x, float y);
		bool isAcid(float x, float y);
		void eraseDoor(float x, float y);

		~Map();

};
