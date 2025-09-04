#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void * destination, int32_t character, uint64_t length);
void *memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
uint8_t strlen(char*);
uint64_t gettime();

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

// Round up x to the next multiple of n (n must be power of 2)
#define ALIGN_UP(x, n) (((x) + ((n) - 1)) & ~((n) - 1))
// Round down x to the previous multiple of n (n must be power of 2)
#define ALIGN_DOWN(x, n) ((x) & ~((n) - 1))

#endif