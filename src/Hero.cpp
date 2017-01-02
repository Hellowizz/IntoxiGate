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

void Hero::changeOrientation(Map m){

	if(m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type == hall){
		cout << "ouest" << endl;
		pos.orientation = ouest;
	}else if(m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type == hall){
		cout << "nord" << endl;
		pos.orientation = nord; //PAS SUR
	}else if(m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type == hall){
		cout << "est" << endl;
		pos.orientation = est;
	}else if(m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type == hall){
		cout << "sud" << endl;
		pos.orientation = sud; //PAS SUR
	}
}

bool Hero::movingForward(Map m){
	//il faudrait vérifier qu'il n'y ai pas de monstre aussi

	if(pos.orientation == sud && m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type != wall)
		return true;
	if(pos.orientation == est && m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type != wall)
		return true;
	if(pos.orientation == nord && m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type != wall)
		return true;
	if(pos.orientation == ouest && m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type != wall)
		return true;

	return false;
}

bool Hero::movingBackward(Map m){
	//il faudrait vérifier qu'il n'y ai pas de monstre aussi
	
	if(pos.orientation == nord && m.pixels[m.width*(pos.pos_Y-1) + pos.pos_X].type != wall)		
		return true;
	if(pos.orientation == est && m.pixels[m.width*pos.pos_Y + (pos.pos_X+1)].type != wall)
		return true;
	if(pos.orientation == sud && m.pixels[m.width*(pos.pos_Y+1) + pos.pos_X].type != wall)	
		return true;
	if(pos.orientation == ouest && m.pixels[m.width*pos.pos_Y + (pos.pos_X-1)].type != wall)
		return true;

	return false;
}


Hero::~Hero(){}