#include <naiveConsole.h>
#include <lib.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t *const videoBase = (uint8_t *)0xB8000;
static uint8_t *videoCurrent = (uint8_t *)0xB8000;
static const uint32_t videoWidth = 80;
static const uint32_t videoHeight = 25 ;
static const uint32_t videoSize = videoWidth * videoHeight;
// static unsigned short *const videoMax = videoBase + videoSize;
volatile unsigned char *last_key = (unsigned char *)0x000B8088;

void ncPrint(const char * string, uint8_t newline) {
	for (int i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);

	if (newline == 1) ncNewline();
}

void ncPrintChar(char character) {
	*videoCurrent = character;
	videoCurrent += 2;
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(videoCurrent - videoCurrent) % (videoWidth * 2) != 0);
}

void ncPrintDec(uint64_t value) {
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value) {
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value) {
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    ncPrint(buffer, 0);
}

void ncClear() {
	for (int i = 0; i < videoHeight * videoWidth; i++)
		videoBase[i * 2] = ' ';
	videoCurrent = videoBase;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
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
				i += 1;
			}
			j++;
		}
		k++;
	}
}
