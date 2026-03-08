#ifndef LIB_H
#define LIB_H

#include <stdint.h>

/* cpuVendor: fills a buffer with the CPU vendor string */
void *cpuVendor(char *buf);

uint64 strlen(const char *string);

/* inportb: reads one byte from an I/O port */
uint8 inportb(uint16 port);

/* outportb: writes one byte to an I/O port */
void outportb(uint16 port, uint8 data);

#endif // LIB_H
