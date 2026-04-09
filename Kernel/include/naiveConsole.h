#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void nc_print(const char *string, uint8 newline);
// do not use outside of nc, overflows
// void ncPrintChar(char character);
void nc_newline();
void ncPrintBase(uint64 value, uint32 base, uint8 newline);
void ncClear();
void ncRainbow();

#endif
