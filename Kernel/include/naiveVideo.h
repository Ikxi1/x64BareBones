#ifndef NAIVE_VIDEO_H
#define NAIVE_VIDEO_H

#include <stdint.h>

#define VBEModeInfoBlock   ((uint8_t*)0x0000000000005C00)
#define VBE_XRES           (*(uint16_t*)(VBEModeInfoBlock + 18))
#define VBE_YRES           (*(uint16_t*)(VBEModeInfoBlock + 20))
#define VBE_BPP            (*(uint8_t*)(VBEModeInfoBlock + 25))
#define VBE_PITCH          (*(uint16_t*)(VBEModeInfoBlock + 16)) // BytesPerScanLine
#define VBE_PHYSBASE       (*(uint32_t*)(VBEModeInfoBlock + 40))

void nv_init();
void nv_plot_pixel(uint16_t x, uint16_t y, uint32_t color);
void nv_clear(uint32_t color);

void nv_rainbow();

#endif