#include <iostream>
#include <string> 

#include <Hero.hpp>

using namespace std;

int main(){

	cout << "Coucou voici notre jeu !" << endl;

	Hero ourHero;

	ourHero.loseLife(30);

	cout << ourHero.life << endl;

	ourHero.loseLife(90);

	cout << ourHero.life << endl;

	return 0;
}