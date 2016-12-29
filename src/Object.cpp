#include <Object.hpp>

Object::~Object() {
	id = 0;
	pos = Position();
	name = "";
	texture = 0;
}