#include <stdint.h>
// #include <string.h>
#include <lib.h>
// #include <moduleLoader.h>
#include <naiveConsole.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

// static void * const sampleCodeModuleAddress = (void*)0x400000;
// static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {
	// char buffer[10];

	// ncPrint("[x64BareBones]", 1);

	// ncPrint("CPU Vendor:", 0);
	// ncPrint(cpuVendor(buffer), 1);

	// ncPrint("[Loading modules]", 1);
	// void * moduleAddresses[] = {
	// 	sampleCodeModuleAddress,
	// 	sampleDataModuleAddress
	// };

	// loadModules(&endOfKernelBinary, moduleAddresses);
	// ncPrint("[Done]", 1);
	// ncNewline();

	// ncPrint("[Initializing kernel's binary]", 1);

	// clearBSS(&bss, &endOfKernel - &bss);

	// ncPrint("  text: 0x", 0);
	// ncPrintHex((uint64_t)&text);
	// ncNewline();
	// ncPrint("  rodata: 0x", 0);
	// ncPrintHex((uint64_t)&rodata);
	// ncNewline();
	// ncPrint("  data: 0x", 0);
	// ncPrintHex((uint64_t)&data);
	// ncNewline();
	// ncPrint("  bss: 0x", 0);
	// ncPrintHex((uint64_t)&bss);
	// ncNewline();

	// ncPrint("[Done]", 1);
	// ncNewline();
	return getStackBase();
}

int main() {

	// ncPrint("[Kernel Main]");
	// ncNewline();
	// ncPrint("  Sample code module at 0x");
	// ncPrintHex((uint64_t)sampleCodeModuleAddress);
	// ncNewline();
	// ncPrint("  Calling the sample code module returned: ");
	// ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	// ncNewline();

	// ncNewline();
	// ncPrint("  Sample data module at 0x");
	// ncPrintHex((uint64_t)sampleDataModuleAddress);
	// ncNewline();
	// ncPrint("  Sample data module contents: ");
	// ncPrint((char*)sampleDataModuleAddress);
	// ncNewline();
	// ncPrint("[Finished]");

	ncPrint("YO this is SICK", 1);
	ncPrint("EPIC I TELL YOU", 1);
	ncPrint("How about.... some... 'fun'.. HAHAHA", 1);

	for (int i = 0; i < 20; ++i) ncPrintDec(gettime(), 1);

	// draw_rainbow();

	return 0;
}
