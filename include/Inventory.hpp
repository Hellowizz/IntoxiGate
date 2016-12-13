#include <vector>

#pragma once

class Inventory{
public:
	std::vector<Weapon> weapons;

	Inventory();

	Inventory::addWeapon(Weapon w);

	~Iventory();
};