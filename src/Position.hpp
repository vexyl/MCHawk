#ifndef POSITION_H_
#define POSITION_H_

struct Position {
	unsigned short x, y, z;

	Position(unsigned short x = 0, unsigned short y = 0, unsigned short z = 0) : x(x), y(y), z(z) {}
	Position(const Position& src) : x(src.x), y(src.y), z(src.z) {}
};

#endif // POSITION_H_
