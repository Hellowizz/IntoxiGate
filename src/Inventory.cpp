#include <Inventory.hpp>

Inventory::Inventory(){}

Inventory::~Inventory(){}

void Inventory::addWeapon(Weapon w){
	weapons.push_back(w);
}

void Inventory::addObject(Object o){
	objects.push_back(o);
}
