#ifndef NAIVE_KEYBOARD_H
#define NAIVE_KEYBOARD_H

#include <stdint.h>
#include <lib.h>

// extern char kb_char;       // ASCII of last key pressed
// extern volatile uint8 key_ready;    // flag: 1 = new key pressed

// extern uint32 key_lut[0xFF];
// extern uint32 shift_key_lut[0xFF];
// extern uint32 alt_gr_key_lut[0xFF];

// extern uint32 released_key_lut[0xFF];

/* init_LUT

initializes all the LUTs for the key presses.
*/
void init_LUT();

/* keyb_irq

Interrupt Request Handler / Interrupt Service Routine
for the keyboard.
*/
void keyb_irq();

/* build_key_event

Build KEY_EVENT from the circular buffer.
*/
void build_key_event();

#define KEY_DEXTENDED 0b0000001000000000
#define KEY_EXTENDED  0b0000000100000000
#define KEY_CTRL      0b0000000010000000
#define KEY_SHIFT     0b0000000001000000
#define KEY_ALT       0b0000000000100000
#define KEY_ALT_GR    0b0000000000010000
#define KEY_CAPS      0b0000000000001000
#define KEY_PRESSED   0b0000000000000100
#define KEY_HELD      0b0000000000000010
#define KEY_RELEASED  0b0000000000000001

typedef struct key_event{
    uint16 flags;
    uint32 key;
} KEY_EVENT;

#define CIRC_BUF_LEN 0xF

typedef struct circ_buf{
    uint32 buffer[CIRC_BUF_LEN];
    uint8 head;
    uint8 tail;
    uint8 length;
} CIRC_BUF;

extern KEY_EVENT key_event;

#endif // NAIVE_KEYBOARD_H
