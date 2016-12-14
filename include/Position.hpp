#pragma once

enum OrientationType { nord, est, sud, ouest };

class Position{
public:
	int pos_X;
	int pos_Y;
	int orientation;

	Position();
	Position(int x, int y);
	void move(int x, int y);

	~Position();
};