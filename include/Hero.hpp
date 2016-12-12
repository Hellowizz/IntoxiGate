#pragma once

#include <Position.hpp>
#include <Character.hpp>

class Hero : public Character{
public:
	int lvl;
	int experience;
	int experienceMax;

	Hero();

	void upLife(int gain);
	void gainExp(int exp);
	void lvlUp();

	~Hero();
};