#include "Utils.hpp"
#include <time.h>
#include <math.h>
#include <string>
#include <openssl/rand.h>

namespace Utils {

// http://www.cplusplus.com/reference/ctime/strftime/
// https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
const std::string CurrentDateTime() {
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F %T", timeinfo);

	return std::string(buffer);
}

// http://www.cplusplus.com/reference/ctime/strftime/
// https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
const std::string CurrentDate() {
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F", timeinfo);

	return std::string(buffer);
}

// http://www.cplusplus.com/reference/ctime/strftime/
// https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
const std::string CurrentTime() {
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%T", timeinfo);

	return std::string(buffer);
}

// For server list response from heartbeat
// Ignores the data curl gives us
size_t write_callback_nop(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	// Shhh compiler
	(void)ptr;
	(void)userdata;

	return size  * nmemb;
}

int32_t Distance3d(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2)
{
	int32_t distance;

	int32_t dx = x2 - x1;
	int32_t dy = y2 - y1;
	int32_t dz = z2 - z1;

	distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

	return distance;
}

bool DistanceCheck(int32_t distance, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2)
{
	return (pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2)) >= pow(distance, 2);
}

// Uses RAND_bytes from openssl library
// Shouldn't have modulo bias
unsigned int GetRandomUInt(unsigned int n)
{
	unsigned char bytes[4];
	unsigned int i;
	unsigned int rand_max = pow(2, sizeof(bytes) * 8) - 1;

	do {
		if (RAND_bytes(bytes, sizeof(bytes)) == 0) {
			fprintf(stderr, "RAND_bytes failed");
			exit(1);
		}

		i = (unsigned int)(*bytes); // is this undefined behavior?
	} while (i > (rand_max - (rand_max % n)));
	
	return i % n;
}

std::string GetRandomSalt()
{
	std::string salt;
	std::string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	for (int i = 0; i < 16; ++i)
		salt += str[GetRandomUInt(str.length())];

	return salt;
}

} // namespace Utils
