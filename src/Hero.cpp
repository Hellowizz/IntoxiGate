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
	if(m.pixels[pos.pos_X+1 + m.width*pos.pos_Y].type == 0){
		pos.orientation = nord;
	}else if(m.pixels[pos.pos_X + m.width*(pos.pos_Y+1)].type == 0){
		pos.orientation = est;
	}else if(m.pixels[pos.pos_X-1 + m.width*pos.pos_Y].type == 0){
		pos.orientation = sud;
	}else if(m.pixels[pos.pos_X + m.width*(pos.pos_Y-1)].type == 0){
		pos.orientation = ouest;
	}
}


Hero::~Hero(){}