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

void Hero::attackMonster(){
	// Auparavant il y a un if sur la touche d'attaque (s'il attaque)
	// Parcours de la liste des monstres, s'il y a un monstre dans la case de devant :
	Monster m;
	m.loseLife(attack + currentWeapon.damage);
}

void Hero::changeOrientation(Map m){
	if(m.pixels[m.width*pos.pos_X + (pos.pos_Y+1)].type == hall){
		pos.orientation = est;
	}else if(m.pixels[m.width*(pos.pos_X+1) + pos.pos_Y].type == hall){
		pos.orientation = sud;
	}else if(m.pixels[m.width*pos.pos_X + (pos.pos_Y-1)].type == hall){
		pos.orientation = ouest;
	}else if(m.pixels[m.width*(pos.pos_X-1) + pos.pos_Y].type == hall){
		pos.orientation = nord;
	}
}

bool Hero::movingForward(Map m){

	cout << "(" << pos.pos_X << ", " << pos.pos_Y << ")" << endl;

	cout << "au nord j'ai une case de type " << m.pixels[m.width*(pos.pos_X-1) + pos.pos_Y].type << endl;
	cout << "à l'est j'ai une case de type " << m.pixels[m.width*pos.pos_X + (pos.pos_Y+1)].type << endl;
	cout << "au sud j'ai une case de type " << m.pixels[m.width*(pos.pos_X+1) + pos.pos_Y].type << endl;
	cout << "à l'ouest j'ai une case de type " << m.pixels[m.width*pos.pos_X + (pos.pos_Y-1)].type << endl;


	if(pos.orientation == nord && (m.pixels[m.width*(pos.pos_X-1) + pos.pos_Y].type == hall || m.pixels[m.width*(pos.pos_X-1) + pos.pos_Y].type == getIn))
		return true;
	if(pos.orientation == est && (m.pixels[m.width*pos.pos_X + (pos.pos_Y+1)].type == hall || m.pixels[m.width*pos.pos_X + (pos.pos_Y+1)].type == getIn))
		return true;
	if(pos.orientation == sud && (m.pixels[m.width*(pos.pos_X+1) + pos.pos_Y].type == hall || m.pixels[m.width*(pos.pos_X+1) + pos.pos_Y].type == getIn))
		return true;
	if(pos.orientation == ouest && (m.pixels[m.width*pos.pos_X + (pos.pos_Y-1)].type == hall || m.pixels[m.width*pos.pos_X + (pos.pos_Y-1)].type == getIn))
		return true;

	return false;
}


Hero::~Hero(){}