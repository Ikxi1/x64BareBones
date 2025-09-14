#include <naiveConsole.h>
// #include <lib.h>

static uint32 uintToBase(uint64 value, char * buffer, uint32 base);

static char buffer[64] = { '0' };
static uint8 *const videoBase = (uint8 *)0xB8000;
static uint8 *videoCurrent = (uint8 *)0xB8000;
static const uint32 videoWidth = 80;
static const uint32 videoHeight = 25 ;
static const uint32 videoSize = videoWidth * videoHeight;

void ncPrint(const char * string, uint8 newline) {
    for (int i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);

    if (newline == 1) ncNewline();
}

void ncPrintChar(char character) {
    *videoCurrent = character;
    videoCurrent += 2;
}

void ncNewline() {
    do {ncPrintChar(' ');}
    while((uint64)(videoCurrent - videoCurrent) % (videoWidth * 2) != 0);
}

void ncPrintDec(uint64 value) {
    ncPrintBase(value, 10);
}

void ncPrintHex(uint64 value) {
    ncPrintBase(value, 16);
}

void ncPrintBin(uint64 value) {
    ncPrintBase(value, 2);
}

void ncPrintBase(uint64 value, uint32 base) {
    uintToBase(value, buffer, base);
    ncPrint(buffer, 0);
}

void ncClear() {
    for (int i = 0; i < videoHeight * videoWidth; i++)
        videoBase[i * 2] = ' ';
    videoCurrent = videoBase;
}

static uint32 uintToBase(uint64 value, char * buffer, uint32 base) {
    char *p = buffer;
    char *p1, *p2;
    uint32 digits = 0;

    //Calculate characters for each digit
    do
    {
        uint32 remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

void draw_rainbow() {
    ncClear();
    unsigned short j = 0;
    int k = 0;
    unsigned background_colour = 0 << 12;
    unsigned foreground_colour = 2 << 8;
    unsigned colour = background_colour | foreground_colour;
    while (1) {
        if (k % 250000000 == 0) {
            for (unsigned short i = 0; i < videoSize;) {
                videoBase[i] = colour | j % 0b0000000011111111;
                i++;
            }
            j++;
        }
        k++;
    }
}
