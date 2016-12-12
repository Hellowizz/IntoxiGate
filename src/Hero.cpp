#include <Hero.hpp>

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

Hero::~Hero(){};