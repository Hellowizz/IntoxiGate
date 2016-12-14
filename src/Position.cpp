#include <Position.hpp>

using namespace std;

Position::Position(){
	pos_X = 0;
	pos_Y = 0;
	orientation = nord;
}

Position::Position(int x, int y) {
	pos_X = x;
	pos_Y = y;
}

void Position::move(int x, int y) {

	pos_X += x;
	pos_Y += y;
}

Position::~Position(){}