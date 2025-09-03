/* _loader.c */
#include <stdint.h>

extern char bss;
extern char endOfBinary;
// extern void memz(void *destiny, uint64_t length);

int main();

void *memset(void * destiny, int8_t c, uint64_t length);

int _start() {
	//Clean BSS
	memset(&bss, '\0', &endOfBinary - &bss);
	// memz(&bss, &endOfBinary - &bss);

	return main();

}


void *memset(void * destiny, int8_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char *dst = (char*)destiny;

	while(length--)
		dst[length] = chr;


	return destiny;
}
