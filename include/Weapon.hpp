#pragma once

#include <Object.hpp>

class Weapon : public Object{
public:
	int damage;

	Weapon();
	Weapon(std::string name, int d);
	~Weapon();

	void usePower();
};