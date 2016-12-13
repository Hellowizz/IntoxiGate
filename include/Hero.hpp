#pragma once

#include <Position.hpp>
#include <Character.hpp>
#include <Weapon.hpp>
#include <Inventory.hpp>

class Hero : public Character{
public:
	int lvl;
	int experience;
	int experienceMax;
	Weapon currentWeapon;
	Inventory inven;

	Hero();

	void upLife(int gain);
	void gainExp(int exp);
	void lvlUp();
	void attackMonster();
	// switchWeapon : Changer d'arme

	~Hero();
};