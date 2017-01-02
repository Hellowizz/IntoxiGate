#pragma once

#include <Map.hpp>
#include <CharacterManager.hpp>
#include <string>

class MapManager{
public:
	Map map;
	Map invertMap;
	CharacterManager cm;
	Square entrance;
	Square entranceInvert;
	int level;

	MapManager();

	void createMap(std::string name);

	~MapManager();
};