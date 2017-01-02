#pragma once

#include <string> 

#include <Position.hpp>

class Object{
public:
	int id;
	Position pos;
	//Position posGraph;

	std::string name;
	int texture;

	~Object();
};