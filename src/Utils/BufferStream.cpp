// From Multiplayer Game Programming: Architecting Networked Games (book; modified)
// https://github.com/MultiplayerBook/MultiplayerBook

#include "BufferStream.hpp"

#include <cstring>
#include <cassert>

#include <iostream>
#include <algorithm>

BufferStream::~BufferStream()
{
	assert(m_buffer != nullptr);
	std::free(m_buffer);
}
void BufferStream::ReallocBuffer(uint32_t inNewLength)
{
	m_buffer = static_cast<char*>(std::realloc(m_buffer, inNewLength));
	m_capacity = inNewLength;
}

uint32_t BufferStream::ReallocIfNeeded(size_t inByteCount)
{
	uint32_t resultHead = m_head + static_cast<uint32_t>(inByteCount);
	if (resultHead > m_capacity)
		ReallocBuffer(std::max(m_capacity * 2, resultHead));
	return resultHead;
}

void BufferStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t resultHead = ReallocIfNeeded(inByteCount);

	std::memcpy(m_buffer + m_head, inData, inByteCount);
	m_head = resultHead;
}

void BufferStream::Read(void* outData, uint32_t inByteCount)
{
	int32_t resultHead = (int32_t)m_head - inByteCount;
	if (resultHead < 0) {
		std::cerr << "Memory access violation" << std::endl;
		exit(1);
	}

	m_head = resultHead;
	std::memcpy(outData, m_buffer + m_head, inByteCount);
}

void BufferStream::WriteZeroes(size_t inByteCount)
{
	uint8_t zero = 0;

	ReallocIfNeeded(inByteCount);
	for (int i = 0; i < (int)inByteCount; ++i)
		Write(zero);
}
