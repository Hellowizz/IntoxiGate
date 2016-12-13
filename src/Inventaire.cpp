#include <Inventaire.hpp>

Inventaire::Inventaire(){}

Inventaire::Inventaire(Weapon w){
	weapons.push_back(w);
}

~Inventaire(){}