#pragma once

#include <Position.hpp>
#include <Character.hpp>
#include <Weapon.hpp>
#include <Inventory.hpp>
#include <Map.hpp>

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
	void changeOrientation(Map m);
	bool movingForward(Map m);

	// switchWeapon : Changer d'arme

	~Hero();
};