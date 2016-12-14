
#include <iostream>
#include <string> 

#include <Hero.hpp>

using namespace std;

Character::Character() {}

void Character::gainLife(int lifeGain){
	life += lifeGain;
}

void Character::loseLife(int lifeLose){
	if(life - lifeLose < 0){
		cout << "Vous avez perdu !!!" << endl;
		exit(0);
	}else{
		life -= lifeLose;	
	}
}

Character::~Character(){}