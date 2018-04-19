#ifndef BUFFERSTREAM_H_
#define BUFFERSTREAM_H_

// From Multiplayer Game Programming: Architecting Networked Games (book; modified)
// https://github.com/MultiplayerBook/MultiplayerBook

#include <cstdint>
#include <cstdlib>
#include <type_traits>

class BufferStream {
public:
	BufferStream(size_t bufferSize=32):
	m_buffer(nullptr), m_head(0), m_capacity(0)
	{ ReallocBuffer(bufferSize); }

	~BufferStream();

	const char* GetBufferPtr() const { return m_buffer; }
	uint32_t GetLength() const { return m_head; }

	void Write(const void* inData, size_t inByteCount);
	void WriteZeroes(size_t inByteCount);
	void Read(void* outData, uint32_t inByteCount);

	template<typename T> void Write(T inData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types");
		BufferStream::Write(&inData, sizeof(inData));
	}

	template<typename T> void Read(T& outData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Read only supports primitive data types");
		BufferStream::Read(&outData, sizeof(outData));
	}

private:
	char* m_buffer;
	uint32_t m_head;
	uint32_t m_capacity;

	void ReallocBuffer(uint32_t inNewLength);
	uint32_t ReallocIfNeeded(size_t inByteCount);
};

#endif // BUFFERSTREAM_H_
