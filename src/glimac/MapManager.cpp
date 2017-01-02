#include <MapManager.hpp>
#include <iostream>

using namespace std;

MapManager::MapManager(){
	level = 0;
}

void MapManager::createMap(string name){
	level++;

	Map mapCreate = Map();
    mapCreate.loadMap(name);
    map = mapCreate;  
    entrance = map.getEntrance();

    cm = CharacterManager();
    cm.heroine = Hero();

    invertMap = map.invert();
    entranceInvert = invertMap.getEntrance();
    cm.heroine.putPos(entranceInvert.pos);

    Waste poubelle;
    poubelle.type = waste;
    poubelle.pos.pos_X = 5;
    poubelle.pos.pos_Y = 9;
    poubelle.posGraph.pos_X = 1;
    poubelle.posGraph.pos_Y = 9;
    cm.monsters.push_back(poubelle);
    cm.heroine.changeOrientation(invertMap);

    cout << "Voici ma position : (" << cm.heroine.pos.pos_X << ", " << cm.heroine.pos.pos_Y << ")" <<  endl;
	
}

MapManager::~MapManager(){}