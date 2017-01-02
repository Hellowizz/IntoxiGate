
#include <iostream>
#include <string> 

#include <Hero.hpp>

using namespace std;

Character::Character() {}

void Character::putPos(Position p){
	pos.pos_X = p.pos_X;
	pos.pos_Y = p.pos_Y;
}

void Character::gainLife(int lifeGain){
	if(life + lifeGain > lifeMax)
		return;
	life += lifeGain;
}

void Character::looseLife(int lifeLose){
	if(life - lifeLose < 0){
		cout << "Vous avez perdu !!!" << endl;
		exit(0);
	}else{
		life -= lifeLose;	
	}
}

Character::~Character(){}