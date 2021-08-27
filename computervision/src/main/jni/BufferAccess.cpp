#include "BufferAccess.h"
#include <string.h>

#ifdef WINDOWS

#ifndef BIG_ENDIAN
#define BIG_ENDIAN    4321  /* to show byte order (taken from gcc) */
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#else
#include <endian.h>
#endif // win32

uint32_t SwapShort(uint16_t a)
{
	a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
	return a;
}

uint32_t SwapWord(uint32_t a)
{
	a = ((a & 0x000000FF) << 24) |
		((a & 0x0000FF00) <<  8) |
		((a & 0x00FF0000) >>  8) |
		((a & 0xFF000000) >> 24);
	return a;
}

uint64_t SwapDWord(uint64_t a)
{
	a = ((a & 0x00000000000000FFULL) << 56) | 
		((a & 0x000000000000FF00ULL) << 40) | 
		((a & 0x0000000000FF0000ULL) << 24) | 
		((a & 0x00000000FF000000ULL) <<  8) | 
		((a & 0x000000FF00000000ULL) >>  8) | 
		((a & 0x0000FF0000000000ULL) >> 24) | 
		((a & 0x00FF000000000000ULL) >> 40) | 
		((a & 0xFF00000000000000ULL) >> 56);
	return a;
}

uint32_t SwapFloat(float f)
{
	uint32_t *pa = (uint32_t*)&f;
	uint32_t a = *pa;
	a = ((a & 0x000000FF) << 24) |
		((a & 0x0000FF00) <<  8) |
		((a & 0x00FF0000) >>  8) |
		((a & 0xFF000000) >> 24);
	return a;
}

float UnswapFloat(uint32_t a)
{
	a = ((a & 0x000000FF) << 24) |
		((a & 0x0000FF00) <<  8) |
		((a & 0x00FF0000) >>  8) |
		((a & 0xFF000000) >> 24);
	float *pf = (float*)&a;
	return *pf;
}

uint64_t SwapDouble(double d)
{
	uint64_t *pa = (uint64_t *)&d;
	uint64_t a = *pa;
	a = ((a & 0x00000000000000FFULL) << 56) | 
		((a & 0x000000000000FF00ULL) << 40) | 
		((a & 0x0000000000FF0000ULL) << 24) | 
		((a & 0x00000000FF000000ULL) <<  8) | 
		((a & 0x000000FF00000000ULL) >>  8) | 
		((a & 0x0000FF0000000000ULL) >> 24) | 
		((a & 0x00FF000000000000ULL) >> 40) | 
		((a & 0xFF00000000000000ULL) >> 56);
	return a;
}

double UnswapDouble(uint64_t a)
{
	a = ((a & 0x00000000000000FFULL) << 56) | 
		((a & 0x000000000000FF00ULL) << 40) | 
		((a & 0x0000000000FF0000ULL) << 24) | 
		((a & 0x00000000FF000000ULL) <<  8) | 
		((a & 0x000000FF00000000ULL) >>  8) | 
		((a & 0x0000FF0000000000ULL) >> 24) | 
		((a & 0x00FF000000000000ULL) >> 40) | 
		((a & 0xFF00000000000000ULL) >> 56);
	double *pd = (double *)&a;
	return *pd;
}

BufferWriter::BufferWriter(const void* buff, unsigned int size)
	:buffer(buff), bufferSize(size)
{
	ptr = (unsigned char*)buffer;
}


BufferWriter::~BufferWriter()
{
}

void BufferWriter::WriteByte( int8_t val )
{
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteUByte( uint8_t val )
{
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteShort( int16_t val )
{
#ifdef LITTLE_ENDIAN
	val = SwapShort(val);
#endif
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteUShort( uint16_t val )
{
#ifdef LITTLE_ENDIAN
	val = SwapShort(val);
#endif
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteInt( int32_t val )
{
#ifdef LITTLE_ENDIAN
	val = SwapWord(val);
#endif
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteUInt( uint32_t val )
{
#ifdef LITTLE_ENDIAN
	val = SwapWord(val);
#endif
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteInt64( int64_t val )
{
#ifdef LITTLE_ENDIAN
	val = SwapDWord(val);
#endif
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteUInt64( uint64_t val )
{
#ifdef LITTLE_ENDIAN
	val = SwapDWord(val);
#endif
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
}

void BufferWriter::WriteFloat( float val )
{	
#ifdef LITTLE_ENDIAN
	uint32_t valint;
	valint = SwapFloat(val);
	memcpy(ptr, &valint, sizeof(valint));
	ptr += sizeof(valint);
#else
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
#endif
}

void BufferWriter::WriteDouble( double val )
{
#ifdef LITTLE_ENDIAN
	uint64_t valint;
	valint = SwapDouble(val);
	memcpy(ptr, &valint, sizeof(valint));
	ptr += sizeof(valint);
#else
	memcpy(ptr, &val, sizeof(val));
	ptr += sizeof(val);
#endif
}

BufferReader::BufferReader( const void* buff, unsigned int size )
	:buffer(buff), bufferSize(size)
{
	ptr = (unsigned char*)buffer;
}

BufferReader::~BufferReader()
{

}

int8_t BufferReader::ReadByte()
{
	int8_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
	return val;
}

uint8_t BufferReader::ReadUByte()
{
	uint8_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
	return val;
}

int16_t BufferReader::ReadShort()
{
	int16_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#ifdef LITTLE_ENDIAN
	val = SwapShort(val);
#endif
	return val;
}

uint16_t BufferReader::ReadUShort()
{
	uint16_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#ifdef LITTLE_ENDIAN
	val = SwapShort(val);
#endif
	return val;
}

int32_t BufferReader::ReadInt()
{
	int32_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#ifdef LITTLE_ENDIAN
	val = SwapWord(val);
#endif
	return val;
}

uint32_t BufferReader::ReadUInt()
{
	uint32_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#ifdef LITTLE_ENDIAN
	val = SwapWord(val);
#endif
	return val;
}

int64_t BufferReader::ReadInt64()
{
	int64_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#ifdef LITTLE_ENDIAN
	val = SwapDWord(val);
#endif
	return val;
}

uint64_t BufferReader::ReadUInt64()
{
	uint64_t val;
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#ifdef LITTLE_ENDIAN
	val = SwapDWord(val);
#endif
	return val;
}

float BufferReader::ReadFloat()
{	
	float val;
#ifdef LITTLE_ENDIAN
	uint32_t valint;
	memcpy(&valint, ptr, sizeof(valint));
	ptr += sizeof(valint);
	val = UnswapFloat(valint);
#else
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#endif
	return val;
}

double BufferReader::ReadDouble()
{
	double val;
#ifdef LITTLE_ENDIAN
	uint64_t valint;
	memcpy(&valint, ptr, sizeof(valint));
	ptr += sizeof(valint);
	val = UnswapDouble(valint);
#else
	memcpy(&val, ptr, sizeof(val));
	ptr += sizeof(val);
#endif
	return val;
}
