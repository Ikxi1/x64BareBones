#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void nc_print(const char *string, uint8 newline);
void nc_print_char(char character);
void nc_newline();
void ncPrintBase(uint64 value, uint32 base, uint8 newline);
void ncClear();
void ncRainbow();

/*
Render cursor square with highlight colour.
Needs to be called each "frame".
*/
void nc_render_cursor();

void nc_delete_char();

extern uint8 cursor_x;
extern uint8 cursor_y;

#endif
