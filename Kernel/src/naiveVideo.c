#include <naiveVideo.h>
#include <naiveConsole.h>
#include <naiveKeyboard.h>

static uint8 *framebuffer = 0;
static uint64 framebuffer_size = 0;
// static uint8 *backbuffer = 0;
static uint16 screen_width = 0;
static uint16 screen_height = 0;
static uint8  screen_bpp = 0;
static uint16 pitch = 0;
extern uint64 *VesaModeInfoBlockBuffer;


void nv_init() {
      struct VBEModeInfo *info = (struct VBEModeInfo*)VesaModeInfoBlockBuffer;

      screen_width     = info->width;
      screen_height    = info->height;
      screen_bpp       = info->bpp;
      pitch            = info->pitch;
      framebuffer      = (uint8*)(uintptr)info->framebuffer; // map physical -> direct linear
      framebuffer_size = screen_height * pitch;
      backbuffer       = (uint8 *)malloc(framebuffer_size);
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


// void nv_rainbow() {
//       static uint8 l = 0;
//       for (uint16 i = 0; i < screen_width; ++i) {
//             for (uint16 j = 0; j < screen_height; ++j) {
//                   nv_plot_pixel(i, j, COLOUR(0,l,0,0));
//             }
//       }
//       ++l;
// }


void nv_rainbow() {
    static uint8 l = 0;

    uint32 color = COLOUR(0, l, 0, 0);
    uint32 *row = (uint32*)framebuffer;

    for (uint16 y = 0; y < screen_height; ++y) {
        uint32 *p = (uint32*)((uint8*)row + y * pitch);

        for (uint16 x = 0; x < screen_width; ++x) {
            p[x] = color;
        }
    }

    ++l;
}


// not ready, i want malloc first to make the second buffer
// can't an array with a runtime variable
void nv_swap_framebuffer() {


}