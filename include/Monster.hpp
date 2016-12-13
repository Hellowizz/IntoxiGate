#pragma once

#include <Position.hpp>
#include <Character.hpp>

class Monster : public Character{
public:
	int id;
 
	int givenExperience;

	//Monstre qui attaque le héro et qui lui fait perdre des pvs
	//Monstre qui meurt et donne de l'expérience au héro

	~Monster();
};