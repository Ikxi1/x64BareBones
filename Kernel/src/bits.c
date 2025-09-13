#include <bits.h>

uint8 getbits8(uint8 x, int8 p, int8 n)
{
	return (x >> p) & ~(~0u << n);
}

uint16 getbits16(uint16 x, int16 p, int16 n)
{
	return (x >> p) & ~(~0u << n);
}

uint32 getbits32(uint32 x, int32 p, int32 n)
{
	return (x >> p) & ~(~0u << n);
}

uint64 getbits64(uint64 x, int64 p, int64 n)
{
	return (x >> p) & ~(~0ull << n);
}

void setbits8(uint8 * x, int8 p, int8 n, uint8 y)
{
	/*Borra los bits que queremos setear*/
	*x &= (~0u << (p+n)) | ~(~0u << p);
	/*Setea los bits*/
	*x |= ((y & ~(~0u << n)) << p);
}

void setbits16(uint16 * x, uint16 p, uint16 n, uint16 y)
{
	*x &= (~0u << (p+n)) | ~(~0u << p);
	*x |= ((y & ~(~0u << n)) << p);
}

void setbits32(uint32 * x, uint32 p, uint32 n, uint32 y)
{
	*x &= (~0u << (p+n)) | ~(~0u << p);
	*x |= ((y & ~(~0u << n)) << p);
}

void setbits64(uint64 * x, uint64 p, uint64 n, uint64 y)
{
	*x &= (~0ull << (p+n)) | ~(~0ull << p);
	*x |= ((y & ~(~0ull << n)) << p);
}

void printbits8(int8 dst[8], uint8 x)
{
	int i;
	for (i=7; i>=0; i--, x = x>>1)
		dst[i] = ((x & 0x01) == 0)? '0' : '1';
}

void printbits16(int8 dst[16], uint16 x)
{
	int i;
	for (i=15; i>=0; i--, x = x>>1)
		dst[i] = ((x & 0x01) == 0)? '0' : '1';
}

void printbits32(int8 dst[32], uint32 x)
{
	int i;
	for (i=31; i>=0; i--, x = x>>1)
		dst[i] = ((x & 0x01) == 0)? '0' : '1';
}

void printbits64(int8 dst[64], uint64 x)
{
	int i;
	for (i=63; i>=0; i--, x = x>>1)
		dst[i] = ((x & 0x01ull) == 0)? '0' : '1';
}

