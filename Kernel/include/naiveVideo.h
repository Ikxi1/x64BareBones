#ifndef NAIVE_VIDEO_H
#define NAIVE_VIDEO_H

#include <stdint.h>

#define COLOUR(r,g,b,a) \
    (((uint8)(r) << 24) | \
    ((uint8)(g) << 16)  | \
    ((uint8)(b) << 8)   | \
    (uint8)(a))

#define VBEModeInfoBlock   ((uint8*)0x0000000000005C00)
#define VBE_XRES           (*(uint16*)(VBEModeInfoBlock + 18))
#define VBE_YRES           (*(uint16*)(VBEModeInfoBlock + 20))
#define VBE_BPP            (*(uint8*)(VBEModeInfoBlock + 25))
#define VBE_PITCH          (*(uint16*)(VBEModeInfoBlock + 16)) // BytesPerScanLine
#define VBE_PHYSBASE       (*(uint32*)(VBEModeInfoBlock + 40))

typedef unsigned long uintptr;

void nv_init();
void nv_plot_pixel(uint16 x, uint16 y, uint32 color);
void nv_clear(uint32 color);
void nv_rainbow();

#endif
