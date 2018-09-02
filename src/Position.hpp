#ifndef POSITION_H_
#define POSITION_H_

struct Position {
	int16_t x, y, z;

	Position(int16_t x = 0, int16_t y = 0, int16_t z = 0) : x(x), y(y), z(z) {}
	Position(const Position& src) : x(src.x), y(src.y), z(src.z) {}
};

#endif // POSITION_H_
