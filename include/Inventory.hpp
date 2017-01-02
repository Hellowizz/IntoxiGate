#pragma once

#include <vector>
#include <Weapon.hpp>
#include <Object.hpp>

using namespace std;

class Inventory{
public:
	vector<Weapon> weapons;
	vector<Object> objects;

	Inventory();
	~Inventory();

	void addWeapon(Weapon w);
	void addObject(Object o);
};