#pragma once

#include <Position.hpp>
#include <Character.hpp>

class Monster : public Character{
public:
	int number;

	int givenExperience;

	Monster();

	~Monster();
};