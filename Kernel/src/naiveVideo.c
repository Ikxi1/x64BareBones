#include <naiveVideo.h>

static uint8 *framebuffer = 0;
static uint16 screen_width = 0;
static uint16 screen_height = 0;
static uint8  screen_bpp = 0;
static uint16 pitch = 0;

extern uint64 *VesaModeInfoBlockBuffer;

void nv_init() {
    struct VBEModeInfo *info = (struct VBEModeInfo*)VesaModeInfoBlockBuffer;

    screen_width  = info->width;
    screen_height = info->height;
    screen_bpp    = info->bpp;
    pitch         = info->pitch;
    framebuffer   = (uint8*)(uintptr)info->framebuffer; // map physical -> direct linear
}

void nv_plot_pixel(uint16 x, uint16 y, uint32 color) {
    if (x < 0 || x >= screen_width || y < 0 || y >= screen_height) return;

    // Assuming 32 bpp (ARGB or BGRA depending on VBE mode)
    uint32 *pixel = (uint32*)(framebuffer + y * pitch + x * (screen_bpp / 8));
    *pixel = color;
}

void nv_clear(uint32 color) {
    for (uint16 y = 0; y < screen_height; y++) {
        for (uint16 x = 0; x < screen_width; x++) {
            nv_plot_pixel(x, y, color);
        }
    }
}

void nv_rainbow() {
    uint8 l = 0;
    uint32 m = 0;
    // while (l < 255) {
    //     for (uint16 i = 0; i < screen_width; ++i) {
    //         for (uint16 j = 0; j < screen_height; ++j) {
    //             nv_plot_pixel(i, j, m);
    //         }
    //     }
    //     ++l;
    //     m = (l/2) << 16 | (l*3/4) << 8 | l;
    // }
    for (uint16 i = 0; i < screen_width; ++i) {
        for (uint16 j = 0; j < screen_height; ++j) {
            nv_plot_pixel(i, j, COLOUR(0, 255, 0, 0));
        }
    }
}
