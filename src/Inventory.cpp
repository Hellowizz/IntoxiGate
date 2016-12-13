#include <Inventory.hpp>

Inventory::Inventory(){}

Inventory::addWeapon(Weapon w){
	weapons.push_back(w);
}

~Inventory(){}