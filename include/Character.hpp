#pragma once

#include <Position.hpp>

class Character{
public:
	int lifeMax;
	int life;
	int attack;
	Position pos;

	Character();

	void putPos(Position p);
	void gainLife(int lifeGain);
	void looseLife(int lifeLose);

	~Character();
};