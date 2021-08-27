
#ifndef BufferAccess_h__
#define BufferAccess_h__

#include <inttypes.h>

class BufferWriter
{
public:
	BufferWriter(const void* buff, unsigned int size);
	~BufferWriter();

	void WriteByte(int8_t val);
	void WriteUByte(uint8_t val);
	void WriteShort(int16_t val);
	void WriteUShort(uint16_t val);
	void WriteInt(int32_t val);
	void WriteUInt(uint32_t val);
	void WriteInt64(int64_t val);
	void WriteUInt64(uint64_t val);
	void WriteFloat(float val);
	void WriteDouble(double val);

private:
	const void* buffer;
	unsigned char* ptr;
	unsigned int bufferSize;
	
};

class BufferReader
{
public:
	BufferReader(const void* buff, unsigned int size);
	~BufferReader();

	int8_t				ReadByte();
	uint8_t				ReadUByte();
	int16_t				ReadShort();
	uint16_t			ReadUShort();
	int32_t				ReadInt();
	uint32_t			ReadUInt();
	int64_t				ReadInt64();
	uint64_t			ReadUInt64();
	float				ReadFloat();
	double				ReadDouble();

private:
	const void* buffer;
	unsigned char* ptr;
	unsigned int bufferSize;

};

#endif // BufferAccess_h__
