#include <MapManager.hpp>
#include <iostream>

using namespace std;

MapManager::MapManager(){
	level = 0;
}

void MapManager::createMap(string name){
	level++;

	Map mapCreate = Map();
	cout<< "ON VA CREER LA MAP..." << endl;
    mapCreate.loadMap(name);
    map = mapCreate;  
    entrance = map.getEntrance();

    cm = CharacterManager();
    cm.heroine = Hero();

    invertMap = map.invert();
    entranceInvert = invertMap.getEntrance();
    cm.heroine.putPos(entranceInvert.pos);
    
    for(unsigned int i= 0 ; i < map.monsters.size(); i++) {
        cm.monsters.push_back(map.monsters[i]);
    }
    cm.heroine.changeOrientation(invertMap);

    cout << "Voici ma position : (" << cm.heroine.pos.pos_X << ", " << cm.heroine.pos.pos_Y << ")" <<  endl;
	
}

MapManager::~MapManager(){}