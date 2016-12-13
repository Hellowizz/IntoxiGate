#pragma once

#include <Position.hpp>
#include <Character.hpp>
#include <Weapon.hpp>

class Hero : public Character{
public:
	int lvl;
	int experience;
	int experienceMax;
	Weapon currentWeapon;
	// Avoir un inventaire

	Hero();

	void upLife(int gain);
	void gainExp(int exp);
	void lvlUp();
	void attackMonster(); // Prendre en compte l'attaque du perso + l'attaque de l'arme
	// switchWeapon : Changer d'arme

	~Hero();
};