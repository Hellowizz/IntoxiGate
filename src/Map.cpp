#include <Map.hpp>

using namespace std;
using namespace string;

void loadObject(string line) {
	int i = 0, id, type;
	string tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	id = stoi(tmp);
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	type = stoi(tmp);
	tmp = "";

	switch(type) {
		case 1:
			Weapon obj;
			break;

		/*case 2:
			Object2 obj;
			break;

		case 3:
			Object3 obj;
			break;

		case 4:
			Object4 obj;
			break;*/

		default:
			Object obj;
	}

	obj.id = id;

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	obj.pos.pos_X = stoi(tmp);
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	obj.pos.pos_Y = stoi(tmp);
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	obj.name = tmp;
	tmp = "";

	while(line[i] != "\n") {
		tmp.append(line[i]);
		i++;
	}
	obj.texture = tmp;
	tmp = "";

	objects.push_back(obj);
}

void loadMonster(string line) {
	int i = 0, id, type;
	string tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	id = stoi(tmp);
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	type = stoi(tmp);
	tmp = "";

	switch(type) {
		case 1:
			Waste mons;
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
			Monster mons;
	}

	mons.id = id;

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	mons.pos.pos_X = stoi(tmp);
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	mons.pos.pos_Y = stoi(tmp);
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	mons.name = tmp;
	tmp = "";

	while(line[i] != ":") {
		tmp.append(line[i]);
		i++;
	}
	mons.life = stoi(tmp);
	tmp = "";

	while(line[i] != "\n") {
		tmp.append(line[i]);
		i++;
	}
	mons.texture = tmp;
	tmp = "";

	monsters.push_back(mons);
}

void loadMap(string fileName) {
	ifstream file;
	string line;
	int nbObj;
	int nbMonst;
	int i;

	file.open (fileName);
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

		while(line[i] != " ") {
			tmp.append(line[i]);
			i++;
		}
		int width = stoi(tmp);
		tmp = "";
		i = 0;

		while(line[i] != " ") {
			tmp.append(line[i]);
			i++;
		}
		int height = stoi(tmp);

		getline(file, line);

		for(i = 0; i < height; i++)
			for(int j = 0; j < width; j++) {
				Square pix;
				pix.pos.pos_X = j;
				pix.pos.pos_Y = i;
				getline(file, line);
				string col;
				strcpy(col, getline(file, line));
				strcat(col, ",");
				getline(file, line);
				strcat(col, line);
				strcat(col, ",");
				getline(file, line);
				strcat(col, line);
				switch(col) {
					case "0,0,0":
						pix.type = wall;
						break;

					case "255,255,255":
						pix.type = hall;
						break;

					case "170,119,34":
						pix.type = door;
						break;

					case "255,90,25":
						pix.type = acid;
						break;

					case "255,0,0":
						pix.type = getOut;
						break;

					case "0,255,0":
						pix.type = getIn;
						break;

					case "0,0,255":
						pix.type = safeRoom;
						break;

					default:
						pix.type = wall;
				}

				*(pixels + i*height + j) = pix;
			}


		file.close();
	}
	else cout << "Unable to open file." << endl;
}