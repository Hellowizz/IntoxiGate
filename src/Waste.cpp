#include <Waste.hpp>

int numberOfMonsters = 0;

Waste::Waste(){
	lifeMax = 150;
	life = lifeMax;
	attack = 5;
	numberOfMonsters ++;
	id = numberOfMonsters;
	givenExperience = 10;

	// Pos = La position qu'il faut
}