#include <cstring>

#include "Packet.hpp"

#include <algorithm>

Packet::Packet(int8_t opcode)
{
		m_opcode = opcode;
		Write(m_opcode);
}

void Packet::Write(const std::string& inData)
{
	int8_t buffer[64];

	memset(buffer, 0x20, sizeof(buffer));
	memcpy(buffer, inData.c_str(), std::min((int)inData.length(), 64));

	BufferStream::Write(buffer, sizeof(buffer));
}

//  Untested
void Packet::Read(std::string& outData)
{
	uint8_t buffer[64];

	memset(buffer, 0x20, sizeof(buffer));
	BufferStream::Read(buffer, sizeof(buffer));

	outData = std::string((char*)buffer, sizeof(buffer));
}
