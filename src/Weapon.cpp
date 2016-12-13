#include <Weapon.hpp>

using namespace std;

Weapon::Weapon() {
	name = "";
	damage = 0;
}

Weapon::Weapon(string n, int d){
	name = n;
	damage = d;
}

Weapon::~Weapon(){}