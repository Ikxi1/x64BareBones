#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string, uint8_t newline);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value, uint8_t newline);
void ncPrintHex(uint64_t value, uint8_t newline);
void ncPrintBin(uint64_t value, uint8_t newline);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

void draw_rainbow();

#endif