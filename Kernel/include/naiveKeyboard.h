#ifndef NAIVE_KEYBOARD_H
#define NAIVE_KEYBOARD_H

#include <stdint.h>
#include <lib.h>

// Index = scan code, Value = ASCII or key code (0 = unused)
extern const uint8_t scancode_lut[128];
extern volatile uint8_t key_ready;
extern uint8_t kb_char;

void keyb_irq();

#endif
