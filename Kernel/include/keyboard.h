#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

enum {SHIFT = 0x1, CONTROL = 0x2, ALT = 0x4};

void initKeyboard();

void keyboardHandler(int k);

uint32 getKeyboardStatus(void);

#endif /*KEYBOARD_H_*/
