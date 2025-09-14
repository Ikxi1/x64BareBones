#include <keyboard.h>
#include <naiveConsole.h>

static uint32 tags = 0;
static char ascii[0xFF];

void initKeyboard() {
    /*Set up the translation table*/
    ascii[0x1E] = 'a'; ascii[0x30] = 'b'; ascii[0x2E] = 'c';
    ascii[0x20] = 'd'; ascii[0x12] = 'e'; ascii[0x21] = 'f';
    ascii[0x22] = 'g'; ascii[0x23] = 'h'; ascii[0x17] = 'i';
    ascii[0x24] = 'j'; ascii[0x25] = 'k'; ascii[0x26] = 'l';
    ascii[0x32] = 'm'; ascii[0x31] = 'n'; ascii[0x18] = 'o';
    ascii[0x19] = 'p'; ascii[0x10] = 'q'; ascii[0x13] = 'r';
    ascii[0x1F] = 's'; ascii[0x14] = 't'; ascii[0x16] = 'u';
    ascii[0x2F] = 'v'; ascii[0x11] = 'w'; ascii[0x2D] = 'x';
    ascii[0x15] = 'y'; ascii[0x2C] = 'z';

    ascii[0x02] = '1'; ascii[0x03] = '2'; ascii[0x04] = '3';
    ascii[0x05] = '4'; ascii[0x06] = '5'; ascii[0x07] = '6';
    ascii[0x08] = '7'; ascii[0x09] = '8'; ascii[0x0A] = '9';
    ascii[0x0B] = '0';
    ascii[0x0C] = '-'; ascii[0x0D] = '='; ascii[0x1A] = '[';
    ascii[0x1B] = ']'; ascii[0x27] = ';'; ascii[0x28] = '\'';
    ascii[0x29] = '`'; ascii[0x2B] = '\\'; ascii[0x33] = ',';
    ascii[0x34] = '.'; ascii[0x35] = '/';

    ascii[0x3B] = '?'; ascii[0x3C] = '?';  ascii[0x3D] = '?';
    ascii[0x3E] = '?'; ascii[0x3F] = '?';  ascii[0x40] = '?';
    ascii[0x41] = '?'; ascii[0x42] = '?';  ascii[0x43] = '?';
    ascii[0x44] = '?'; ascii[0x85] = '?';  ascii[0x86] = '?';

    ascii[0x0E] = '\b';
    ascii[0x1C] = '\n';
    ascii[0x39] = ' ';
}

uint32 getKeyboardStatus() {
    return tags;
}

void keyboardHandler(int k) {
    uint8 sc = *((uint8*)&k);

    // if (sc == 0x2A || k == 0x36) {
    //     tags = tags | SHIFT;
    //     return;
    // }
    // if (sc == 0xAA || sc == 0xB6) { // Shift break
    //     tags=tags & ~SHIFT;
    //     return;
    // }
    // if (sc == 0x1d || k == 0xE036) {
    //     tags = tags | CONTROL;
    //     return;
    // }
    // if (sc == 0x9d || sc == 0xB6) { // Control break
    //     tags = tags & ~CONTROL;
    //     return;
    // }
    // if (sc == 0x38 || sc == 0x54) {
    //     tags = tags | ALT;
    //     return;
    // }
    // if (sc == 0xB8 || sc == 0xD4) { // Alt break
    //     tags = tags & ~ALT;
    //     return;
    // }

    /*Ignore the other release scancodes*/
    if (sc > 0x81)
        return;

    // arrow_left
    // if (sc == 0x4B) {
    //     decInCursor();
    //     return;
    // }
    // arrow_right
    // if (sc == 0x4D) {
    //     incInCursor();
    //     return;
    // }

    //if (ascii[k] == '6' && (tags & SHIFT)) {
    //    feedBuffer('&');
    //    return;
    //}
    // printing here
    // feedBuffer(ascii[k] + ((tags & SHIFT)? ('A' - 'a'):0 ));
    ncPrintChar(ascii[k]);
}
