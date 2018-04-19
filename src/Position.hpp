#ifndef POSITION_H_
#define POSITION_H_

struct Position {
	int16_t x, y, z;

	Position(int16_t inX, int16_t inY, int16_t inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}

	Position() { Position(0, 0, 0); }
};

#endif // POSITION_H_
