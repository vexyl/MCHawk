#ifndef PACKET_H_
#define PACKET_H_

#include <string>

#include "../Utils/BufferStream.hpp"

class Packet : public BufferStream {
public:
	using BufferStream::Write;
	using BufferStream::Read;

	Packet() : m_opcode(0) { }
	Packet(int8_t opcode);

	void Write(const std::string& inData);
	void Read(std::string& outData);

private:
	int8_t m_opcode;
};

#endif // PACKET_H_
