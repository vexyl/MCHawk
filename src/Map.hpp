#ifndef MAP_H_
#define MAP_H_

#define calcMapOffset(x, y, z, xSize, zSize) ((y * zSize + z) * xSize + x)

#include <cstdint>

#include <string>

#include "Position.hpp"

class Map {
public:
  Map();

  ~Map();

  void SetDimensions(const Position& pos);
  void SetFilename(std::string filename);

  uint8_t* GetBuffer() { return m_buffer; }
  size_t GetBufferSize() { return m_bufferSize; }
  int16_t& GetXSize() { return m_x; }
  int16_t& GetYSize() { return m_y; }
  int16_t& GetZSize() { return m_z; }
  std::string GetFilename() { return m_filename; }

  void GenerateFlatMap(std::string filename, short x, short y, short z);

  void Load();
  void LoadFromFile(std::string filename);

  void SaveToFile(std::string filename);
  void SaveToFile() { SaveToFile(m_filename); }

  void SetBlock(Position& pos, uint8_t type);
  uint8_t GetBlockType(short x, short y, short z);

  void CompressBuffer(uint8_t** outCompBuffer, size_t* outCompSize);

private:
  uint8_t *m_buffer;
  size_t m_bufferSize;

  std::string m_filename;

  int16_t m_x, m_y, m_z; // Size
};

#endif // MAP_H_
