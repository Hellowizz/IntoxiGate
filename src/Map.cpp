#include <Map.hpp>

using namespace std;

Map::Map() {
	mapName = "";
	ppmFile = "";
	width = 0;
	height = 0;
}

void Map::loadObject(string line) {

	int i = 0, id, type;
	Object obj;
	string tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	id = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	type = stoi(tmp);
	tmp = "";

	switch(type) {
		case 1:
			obj = Weapon();
			break;

		case 2:
			obj = Key();
			break;

		/*case 3:
			Object3 obj;
			break;

		case 4:
			Object4 obj;
			break;*/

		default:
			obj = Object();
	}

	obj.id = id;

	// Position

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.pos.pos_X = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.pos.pos_Y = stoi(tmp);
	tmp = "";

	// Position graphique

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.posGraph.pos_X = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.posGraph.pos_Y = stoi(tmp);
	tmp = "";


	// Nom de l'objet

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.name = tmp;
	tmp = "";

	// Numéro de texture

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	obj.texture = stoi(tmp);
	tmp = "";

	objects.push_back(obj);
}

void Map::loadMonster(string line) {
	int i = 0, id, type;
	Monster mons;
	string tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	id = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	type = stoi(tmp);
	tmp = "";

	switch(type) {
		case 1: {
			mons = Waste();
		}
		break;

		/*case 2:
			Monster2 mons;
			break;

		case 3:
			Monster3 mons;
			break;

		case 4:
			Monster4 mons;
			break;*/

		default:
			mons = Monster();
	}

	mons.id = id;

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	mons.pos.pos_X = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	mons.pos.pos_Y = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	mons.life = stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	tmp = "";

	monsters.push_back(mons);
}

void Map::loadMap(string fileName) {
	ifstream file;
	string line;
	int nbObj;
	int nbMonst;
	int i;

	file.open(fileName);
	if(file.is_open()) {
		getline(file, line);
		mapName = line;

		getline(file, line);
		ppmFile = line;

		getline(file, line);
		nbObj = stoi(line);
		for(i = 0; i < nbObj; i++) {
			getline(file, line);
			loadObject(line);
		}


		getline(file, line);
		nbMonst = stoi(line);
		for(i = 0; i < nbMonst; i++) {
			getline(file, line);
			loadMonster(line);
		}

		file.close();
	}
	else cout << "Unable to open file." << endl;

	file.open(ppmFile);
	if(file.is_open()) {
		getline(file, line);
		getline(file, line);
		getline(file, line);
		i = 0;
		string tmp = "";

		while(line[i] != ' ') {
			tmp.append(1, line[i]);
			i++;
		}
		i++;
		width = stoi(tmp);
		tmp = "";

		while(line[i] != ' ') {
			tmp.append(1, line[i]);
			i++;
		}
		height = stoi(tmp);
		tmp = "";

		pixels.resize(width*height);

		getline(file, line);

		for(i = 0; i < height; i++){
			for(int j = 0; j < width; j++) {
				Square pix;
				pix.pos.pos_X = i;
				pix.pos.pos_Y = j;

				getline(file, line);
				string col;
				col.append(line);
				col.append(",");
				getline(file, line);
				col.append(line);
				col.append(",");
				getline(file, line); 
				col.append(line);
				
				if(col.compare("0,0,0") == 0)
					pix.type = wall;

				else if(col.compare("255,255,255") == 0)
					pix.type = hall;

				else if(col.compare("170,119,34") == 0)
					pix.type = door;

				else if(col.compare("255,90,25") == 0)
					pix.type = acid;

				else if(col.compare("255,0,0") == 0)
					pix.type = getOut;

				else if(col.compare("0,255,0") == 0)
					pix.type = getIn;

				else if(col.compare("0,0,255") == 0)
					pix.type = safeRoom;

				else
					pix.type = wall;

				pixels[i*width + j] = pix;
			}
		}
		file.close();
	} 
	else{
		cout << "Unable to open file." << endl;
		exit(1);
	} 
}

Square Map::getEntrance() {
	Square ret;

	for(unsigned int i = 0; i < pixels.size(); i++) {
		if(pixels[i].type == 5) {
			return pixels[i];
		}
	}
	cout << "Erreur : pas d'entrée" << endl;
	return ret;	
}

Map Map::invert(){
	Map cpy;

	cpy.mapName = mapName;
	cpy.objects = objects;
	cpy.monsters = monsters;
	cpy.ppmFile = ppmFile;
	cpy.width = width;
	cpy.height = height; 

	/*for(size_t i = 0; i<pixels.size(); i++){
		cpy.pixels.push_back (pixels[pixels.size()-i-1]);
	}*/

	int t = 0;

	for(int j = 0; j<height; j++){
		for(int i = 0; i<width; i++){
			Square s;
			s.pos.pos_X = i;
			s.pos.pos_Y = j;
			s.type = pixels[width*(height-j-1) + (width-i-1)].type;

			cpy.pixels.push_back(s);
			t++;
		}
	}

	return cpy;
}

int Map::isObject(float x, float y) {
	for(unsigned int i = 0; i < objects.size(); i++) {
		if(objects[i].pos.pos_X == x && objects[i].pos.pos_Y == y)
			return i;
	}
	return -1;
}

bool Map::isAcid(float x, float y) {
	for(unsigned int i = 0; i < pixels.size(); i++) {
		if(pixels[i].pos.pos_X == x && pixels[i].pos.pos_Y == y && pixels[i].type == acid)
			return true;
	}
	return false;
}

void Map::eraseDoor(float x, float y) {
	for(unsigned int i = 0; i < pixels.size(); i++) {
		if(pixels[i].pos.pos_X == x && pixels[i].pos.pos_Y == y)
			pixels[i].type = hall;
	}
}

Map::~Map(){}
