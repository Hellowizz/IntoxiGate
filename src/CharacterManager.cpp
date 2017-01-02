#include <iostream>

#include <CharacterManager.hpp>

using namespace std;

CharacterManager::CharacterManager(){}

bool CharacterManager::monsterForward(){
	for(size_t i=0; i<monsters.size(); i++){
		if(heroine.pos.orientation == nord && heroine.pos.pos_X == monsters[i].pos.pos_X && heroine.pos.pos_Y-1 == monsters[i].pos.pos_Y){
			return true;
		}
		else if(heroine.pos.orientation == est && heroine.pos.pos_X-1 == monsters[i].pos.pos_X && heroine.pos.pos_Y == monsters[i].pos.pos_Y)
			return true;
		else if(heroine.pos.orientation == sud && heroine.pos.pos_X == monsters[i].pos.pos_X && heroine.pos.pos_Y+1 == monsters[i].pos.pos_Y){
			return true;
		}
		else if(heroine.pos.orientation == ouest && heroine.pos.pos_X == monsters[i].pos.pos_X+1 && heroine.pos.pos_Y == monsters[i].pos.pos_Y)
			return true;
	}

	return false;
}

bool CharacterManager::monsterBackward(){
	for(size_t i=0; i<monsters.size(); i++){
		if(heroine.pos.orientation == nord && heroine.pos.pos_X == monsters[i].pos.pos_X && heroine.pos.pos_Y+1 == monsters[i].pos.pos_Y)
			return true;
		else if(heroine.pos.orientation == est && heroine.pos.pos_X+1 == monsters[i].pos.pos_X && heroine.pos.pos_Y-1 == monsters[i].pos.pos_Y)
			return true;
		else if(heroine.pos.orientation == sud && heroine.pos.pos_X == monsters[i].pos.pos_X && heroine.pos.pos_Y+1 == monsters[i].pos.pos_Y)
			return true;
		else if(heroine.pos.orientation == ouest && heroine.pos.pos_X == monsters[i].pos.pos_X-1 && heroine.pos.pos_Y == monsters[i].pos.pos_Y)
			return true;
	}

	return false;
}

void CharacterManager::monstersAttack(){
	for(size_t i = 0; i < monsters.size(); i++){

		bool attack = false;

		if(monsters[i].pos.pos_X == heroine.pos.pos_X){
			if(monsters[i].pos.pos_Y == heroine.pos.pos_Y+1 || monsters[i].pos.pos_Y == heroine.pos.pos_Y-1){
				attack = true;
			}		
		}else if(monsters[i].pos.pos_Y == heroine.pos.pos_Y){
			if(monsters[i].pos.pos_X == heroine.pos.pos_X+1 || monsters[i].pos.pos_X == heroine.pos.pos_X-1){
				attack = true;
			}
		}

		if(attack){
			heroine.looseLife(monsters[i].attack);
			if(heroine.life <= 0){
				cout << "Vous êtes mort, veuillez réessayer" << endl;
				exit(5);
			}
			cout << "il ne vous reste plus que : " << heroine.life << " pv" << endl;
		}
	}
}

int CharacterManager::heroAttack(){
	for(size_t i = 0; i < monsters.size(); i++){

		bool attack = false;

		if(monsters[i].pos.pos_X == heroine.pos.pos_X){
			if((monsters[i].pos.pos_Y == heroine.pos.pos_Y+1 && heroine.pos.orientation == sud) || (monsters[i].pos.pos_Y == heroine.pos.pos_Y-1 && heroine.pos.orientation == nord)){
				attack = true;
				break;
			}		
		}else if(monsters[i].pos.pos_Y == heroine.pos.pos_Y){
			if((monsters[i].pos.pos_X == heroine.pos.pos_X+1 && heroine.pos.orientation == ouest)|| (monsters[i].pos.pos_X == heroine.pos.pos_X-1 && heroine.pos.orientation == est)){
				attack = true;
				break;
			}
		}
	}
	
	if(attack) {
		monsters[i].looseLife(heroine.attack);
		cout << "Le monstre n'a plus que " << monsters[i].life << " pv" << endl;
		return heroine.pos.orientation;
	}

	return -1;
}

void CharacterManager::moveAllMonsters(){
	for(size_t i=0; i<monsters.size(); i++){
        monsters[i].move(heroine.pos);
    }
}

CharacterManager::~CharacterManager(){}