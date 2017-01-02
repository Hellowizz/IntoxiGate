#include <Monster.hpp>
#include <iostream>


Monster::Monster() {}

void Monster::move(Position heroPos){

	if (heroPos.pos_Y == pos.pos_Y){
		if(heroPos.pos_X == pos.pos_X+2 || heroPos.pos_X == pos.pos_X+3){
			pos.pos_X ++;
			posGraph.pos_Y --;
		}else if(heroPos.pos_X == pos.pos_X-2 || heroPos.pos_X == pos.pos_X-3){
			pos.pos_X --;
			posGraph.pos_Y ++;
		}
	}
	if (heroPos.pos_X == pos.pos_X){
		if(heroPos.pos_Y == pos.pos_Y+2 || heroPos.pos_Y == pos.pos_Y+3){
			pos.pos_Y ++;
			posGraph.pos_X --;
		}else if(heroPos.pos_Y == pos.pos_Y-2 || heroPos.pos_Y == pos.pos_Y-3){
			pos.pos_Y --;
			posGraph.pos_X ++;
		}
	}
}

void Monster::looseLife(int lifeLose) {
	life -= lifeLose;	
}

Monster::~Monster() {}