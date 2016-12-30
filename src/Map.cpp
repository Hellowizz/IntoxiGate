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

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.pos.pos_X = (float)stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.pos.pos_Y = (float)stoi(tmp);
	tmp = "";

	while(line[i] != ':') {
		tmp.append(1, line[i]);
		i++;
	}
	i++;
	obj.name = tmp;
	tmp = "";

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
	mons.name = tmp;
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
	mons.texture = tmp;
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

		pixels.resize(width*height);

		getline(file, line);

		for(i = 0; i < height; i++)
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

		file.close();
		cout << width << "," << height << endl;
	} 
	else cout << "Unable to open file." << endl;
}

Square Map::getEntrance() {
	Square ret;
	cout << pixels.size() << endl;

	for(unsigned int i = 0; i < pixels.size(); i++) {
		if(pixels[i].type == 5) {
			return pixels[i];
		}
	}
	cout << "Erreur : pas d'entrée" << endl;
	return ret;	
}

int Map::isObject(float x, float y) {
	for(unsigned int i = 0; i < objects.size(); i++) {
		cout << "Position héro : " << x << ", " << y << endl;
		cout << "Position objet : " << objects[i].pos.pos_X << ", " << objects[i].pos.pos_Y << endl;
		if(objects[i].pos.pos_X == x && objects[i].pos.pos_Y == y)
			return i;
	}
	return -1;
}