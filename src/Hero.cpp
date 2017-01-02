#include <Hero.hpp>
#include <Monster.hpp>

using namespace std;

Hero::Hero() {
	lvl = 1;
	experience = 0;
	experienceMax = 100;
	lifeMax = 100;
	life = 100;
	attack = 20;
}

void Hero::upLife(int gain){
		life += gain;
}

void Hero::looseLife(int pain){
		life -= pain;
}

void Hero::gainExp(int exp){
	if(experience + exp >= experienceMax){
		lvlUp();
		experience = experience + exp - experienceMax;
	}else{
		experience += exp;
	}
}

void Hero::lvlUp(){
	lvl ++;
	upLife(20);
}

bool Hero::atTheEndOfTheDungeon(Map m){
	if(m.pixels[m.width*pos.pos_Y + (pos.pos_X)].type == getOut)
		return true;
	return false;
}

void Hero::changeOrientation(Map m){

	if(m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type == hall){
		pos.orientation = ouest;
	}else if(m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type == hall){
		pos.orientation = sud; //PAS SUR
	}else if(m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type == hall){
		pos.orientation = est;
	}else if(m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type == hall){
		pos.orientation = nord; //PAS SUR
	}
}

int Hero::movingForward(Map m){

	if(pos.orientation == nord && m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type != wall) {
		if(m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}
	if(pos.orientation == est && m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type != wall){
		if(m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}
	if(pos.orientation == sud && m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type != wall){
		if(m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}
	if(pos.orientation == ouest && m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type != wall){
		if(m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}

	return 0;
}

int Hero::movingBackward(Map m){
	if(pos.orientation == nord && m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type != wall){
		if(m.pixels[m.width*pos.pos_X + (pos.pos_Y+1)].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}
	if(pos.orientation == est && m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type != wall){
		if(m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}
	if(pos.orientation == sud && m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type != wall){
		if(m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}
	if(pos.orientation == ouest && m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type != wall){
		if(m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type == door) {
			if(!hasKey())
				return 0;
			useKey();
			return 2;
		}
		return 1;
	}

	return 0;
}

bool Hero::hasKey() {
	unsigned int i;
	for(i = 0; i < inven.objects.size(); i++) {
		if(inven.objects[i].name.compare("key") == 0) {
			return true;
		}
	}
	return false;
}

void Hero::useKey() {
	unsigned int i;
	for(i = 0; i < inven.objects.size(); i++) {
		if(inven.objects[i].name.compare("key") == 0)
			inven.objects.erase(inven.objects.begin()+i);
	}
}


Hero::~Hero(){}