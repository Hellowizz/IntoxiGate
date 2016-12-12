#pragma once

enum OrientationType { nord, sud, est, ouest };

class Position{
public:
	int pos_X;
	int pos_Y;
	OrientationType orientation;

	Position();
	Position(int x, int y);
	void move(int x, int y);

	~Position();
};