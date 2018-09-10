#ifndef POSITION_H_
#define POSITION_H_

#include <cstdint>

struct Position {
  int16_t x, y, z;

  Position(int16_t inX = 0, int16_t inY = 0, int16_t inZ = 0) : x(inX), y(inY), z(inZ) {}
  Position(const Position& src) : x(src.x), y(src.y), z(src.z) {}
};

#endif // POSITION_H_
