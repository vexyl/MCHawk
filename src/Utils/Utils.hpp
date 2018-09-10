#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>

#include <string>

namespace Utils {

// http://www.cplusplus.com/reference/ctime/strftime/
// https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
const std::string CurrentDate();
const std::string CurrentTime();

int32_t Distance3d(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
bool DistanceCheck(int32_t distance, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);

unsigned int GetRandomUInt(unsigned int n);
std::string GetRandomSalt();

} // namespace Utils

#endif // UTILS_H_
