#include <Monster.hpp>

int nbMonsters = 0;

Monster::Monster{
	nbMonsters ++;
	id = nbMonsters;
}