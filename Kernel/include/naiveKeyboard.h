#ifndef NAIVE_KEYBOARD_H
#define NAIVE_KEYBOARD_H

#include <stdint.h>
#include <lib.h>

extern char kb_char;       // ASCII of last key pressed
extern volatile uint8 key_ready;    // flag: 1 = new key pressed

uint32 key_lut[0xFF];

void keyb_irq(void);

#endif
