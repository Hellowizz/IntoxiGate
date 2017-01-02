#pragma once

//#include <Hero.hpp>
#include <Position.hpp>
#include <Character.hpp>
#include <string>

using namespace std;

enum monsterType { waste };

class Monster : public Character{
public:
	int id;
 
 	monsterType type;
	int givenExperience;
	Position posGraph;
	string name;
	int text;

	Monster();

	void move(Position heroPos);
	void looseLife(int lifeLose);

	~Monster();
	//Monstre qui meurt et donne de l'expérience au héro
};