#include "Map.hpp"

#include <assert.h>
#include <string.h>
#include <zlib.h>

#include "Server.hpp" // For logging

#ifdef __linux__
	#include <arpa/inet.h>
#elif _WIN32
	#include <winsock2.h>
#endif

Map::Map() : m_buffer(nullptr)
{
	SetDimensions(0, 0, 0);
}

Map::~Map()
{
	if (m_buffer != nullptr)
		free(m_buffer);
}

void Map::SetDimensions(short x, short y, short z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void Map::SetFilename(std::string filename)
{
	m_filename = filename;
}

void Map::Load()
{
	LoadFromFile(m_filename);
}

// TODO: Use C++ file streams
void Map::LoadFromFile(std::string filename)
{
	if (m_buffer != nullptr)
		delete m_buffer;


	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr) {
		LOG(LogLevel::kError, "Can't open map file for reading");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	m_bufferSize = ftell(fp);
	rewind(fp);

	m_buffer = (uint8_t*)malloc(sizeof(uint8_t) * m_bufferSize);
	if (m_buffer == nullptr) {
		LOG(LogLevel::kError, "Couldn't allocate memory for map buffer");
		exit(1);
	}

	if (fread(m_buffer, sizeof(uint8_t), m_bufferSize, fp) != m_bufferSize) {
		LOG(LogLevel::kError, "Couldn't read map from file");
		exit(1);
	}

	fclose(fp);

	LOG(LogLevel::kInfo, "Loaded map file %s (%d bytes)", filename.c_str(), m_bufferSize);
}

void Map::GenerateFlatMap(std::string filename, short x, short y, short z)
{
	m_filename = filename;

	SetDimensions(x, y, z);

	m_bufferSize = x*y*z+4;
	m_buffer = (uint8_t*)malloc(sizeof(uint8_t) * m_bufferSize);

	memset(m_buffer, 0, m_bufferSize);

	int sz = htonl(m_bufferSize-4);
	memcpy(m_buffer, &sz, sizeof(sz));

	for (short gen_y = 0; gen_y < y/2; gen_y++) {
		for (short gen_x = 0; gen_x < x; gen_x++) {
			for (short gen_z = 0; gen_z < z; gen_z++) {
				if (gen_y < (y/2 - 1))
					SetBlock(gen_x, gen_y, gen_z, 0x03);
				else
					SetBlock(gen_x, gen_y, gen_z, 0x02);
			}
		}
	}

	LOG(LogLevel::kDebug, "Generated flat map '%s'", m_filename.c_str());
}

// TODO: Use C++ file streams
void Map::SaveToFile(std::string filename)
{
	FILE *fp = fopen(filename.c_str(), "wb");
	if (fp == nullptr) {
		LOG(LogLevel::kError, "Can't open map file for writing");
		exit(1);
	}

	if (fwrite(m_buffer, sizeof(uint8_t), m_bufferSize, fp) != m_bufferSize) {
		LOG(LogLevel::kError, "Couldn't write map to file");
		exit(1);
	}

	fclose(fp);

	LOG(LogLevel::kDebug, "Saved map file %s (%d bytes)", filename.c_str(), m_bufferSize);
}

bool Map::SetBlock(short x, short y, short z, int8_t type)
{
	int offset = calcMapOffset(x, y, z, m_x, m_z) + 4;

	if ((offset + 1) > (int)m_bufferSize) {
		LOG(LogLevel::kWarning, "Attempted map write passed buffer size");
		return false;
	}

	m_buffer[offset] = type;

	return true;
}

// block type is -1 on error
Map::Block Map::GetBlock(short x, short y, short z)
{
	int offset = calcMapOffset(x, y, z, m_x, m_z) + 4;
	if (offset < (int)m_bufferSize)
		return Block(x, y, z, m_buffer[offset]);

	return Block();
}

// returns -1 on error
int8_t Map::GetBlockType(short x, short y, short z)
{
	int offset = calcMapOffset(x, y, z, m_x, m_z) + 4;

	if ((offset + 1) > (int)m_bufferSize)
		return -1;

	return m_buffer[offset];
}

void Map::CompressBuffer(uint8_t** outCompBuffer, size_t* outCompSize)
{
	assert(*outCompBuffer==nullptr && m_buffer != nullptr);

	*outCompBuffer = (uint8_t*)malloc(sizeof(uint8_t) * m_bufferSize);
	if (*outCompBuffer == nullptr) {
		LOG(LogLevel::kError, "Couldn't allocate memory for map buffer");
		exit(1);
	}

	memset(*outCompBuffer, 0, m_bufferSize);

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = (uLong)m_bufferSize;
	strm.next_in = (Bytef*)m_buffer;
	strm.avail_out = 0;
	strm.next_out = Z_NULL;

	int ret = deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, (MAX_WBITS + 16), 8, Z_DEFAULT_STRATEGY);
	if (ret != Z_OK) {
		LOG(LogLevel::kDebug, "Zlib error: deflateInit2()");
		exit(1);
	}

	strm.avail_out = (uLong)m_bufferSize;
	strm.next_out = (Bytef*)(*outCompBuffer);

	ret = deflate(&strm, Z_FINISH);

	switch (ret) {
		case Z_NEED_DICT:
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			LOG(LogLevel::kError, "Zlib error: inflate()");
			exit(1);
	}

	deflateEnd(&strm);

	*outCompSize = (size_t)strm.total_out;
}
