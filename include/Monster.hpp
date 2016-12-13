#pragma once

#include <Position.hpp>
#include <Character.hpp>
#include <string>

class Monster : public Character{
public:
	int id;
 
 	string name;
 	string texture;
	int givenExperience;

	//Monstre qui attaque le héro et qui lui fait perdre des pvs
	//Monstre qui meurt et donne de l'expérience au héro

	~Monster();
};