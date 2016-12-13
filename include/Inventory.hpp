#pragma once

#include <vector>
#include <Weapon.hpp>

using namespace std;

class Inventory{
public:
	vector<Weapon> weapons;

	Inventory();
	~Inventory();

	void addWeapon(Weapon w);
};