#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t *const videoBase = (uint8_t *)0xB8000;
static uint8_t *videoCurrent = (uint8_t *)0xB8000;
static const uint32_t videoWidth = 80;
static const uint32_t videoHeight = 25 ;
static const uint32_t videoSize = videoWidth * videoHeight;
static uint8_t *const videoMax = videoBase + videoSize;

void ncPrint(const char * string, uint8_t newline) {
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);

	if (newline == 1) ncNewline();
}

void ncPrintChar(char character) {
	*videoCurrent = character;
	videoCurrent += 2;
}

void ncNewline() {
	while((uint64_t)(videoCurrent - videoBase) % (videoWidth * 2) != 0)	{
		ncPrintChar(' ');
		if (videoCurrent - videoBase);
	}
}

void ncPrintDec(uint64_t value, uint8_t newline) {
	ncPrintBase(value, 10);
	if (newline == 1) ncNewline();
}

void ncPrintHex(uint64_t value, uint8_t newline) {
	ncPrintBase(value, 16);
	if (newline == 1) ncNewline();
}

void ncPrintBin(uint64_t value, uint8_t newline) {
	ncPrintBase(value, 2);
	if (newline == 1) ncNewline();
}

void ncPrintBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    ncPrint(buffer, 0);
}

void ncClear() {
	int i;

	for (i = 0; i < videoSize; i++)
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
	for (int i = 0; i < videoSize; ++i) {
		videoBase[i * 2] = i % 255;
	}
}