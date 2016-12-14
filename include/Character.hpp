#pragma once

#include <Position.hpp>

class Character{
public:
	int lifeMax;
	int life;
	int attack;
	Position pos;

	Character();

	void gainLife(int lifeGain);
	void loseLife(int lifeLose);

	~Character();
};