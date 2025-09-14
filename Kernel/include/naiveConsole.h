#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char *string, uint8 newline);
void ncPrintChar(char character);
void ncNewline();
void ncPrintBase(uint64 value, uint32 base, uint8 newline);
void ncClear();

#endif
