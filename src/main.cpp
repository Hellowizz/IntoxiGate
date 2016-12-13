#include <iostream>
#include <string> 

#include <Hero.hpp>
#include <Map.hpp>

using namespace std;

int main(){

	cout << "Coucou voici notre jeu !" << endl;

	Hero ourHero;

	Map newMap;
	newMap.loadMap("./maps/level1.txt");

	ourHero.loseLife(30);

	cout << ourHero.life << endl;

	ourHero.loseLife(90);

	cout << ourHero.life << endl;

	// Boucle while (tant qu'il y a des events)
		// if(flèche) > changer position et orientation en fonction
		// if(un monstre le detecte) > le monstre avance vers lui
		// if (touche pour changer d'arme) > switch d'arme
		// if(touche d'attaque) > attaquer 
			// if(l'attaque touche un mob) > mob.perdDeLaVie(attaqueHero)
			// if(UnMonstre attaque && touche le perso) > perso.PerdVie(attaqueMonstre)

	return 0;
}