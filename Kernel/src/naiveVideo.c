#include <naiveVideo.h>

static uint8_t *framebuffer = 0;
static uint16_t screen_width = 0;
static uint16_t screen_height = 0;
static uint8_t  screen_bpp = 0;
static uint16_t pitch = 0;

void nv_init() {
    screen_width  = VBE_XRES;
    screen_height = VBE_YRES;
    screen_bpp    = VBE_BPP;
    pitch         = VBE_PITCH;
    framebuffer   = (uint8_t*)(uintptr_t)VBE_PHYSBASE; // map physical -> direct linear

    // Optional: simple check
    if (screen_bpp != 32) {
        // Pure64 usually sets 32 bpp, but warn if not
        // You could add code for 24/16 bpp
    }
}

void nv_plot_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x < 0 || x >= screen_width || y < 0 || y >= screen_height) return;

    // Assuming 32 bpp (ARGB or BGRA depending on VBE mode)
    uint32_t *pixel = (uint32_t*)(framebuffer + y * pitch + x * (screen_bpp / 8));
    *pixel = color;
}

void nv_clear(uint32_t color) {
    for (uint16_t y = 0; y < screen_height; y++) {
        for (uint16_t x = 0; x < screen_width; x++) {
            nv_plot_pixel(x, y, color);
        }
    }
}

void nv_rainbow() {
	uint8_t l = 0;
	uint32_t m = 0;
	while (l < 255) {
		for (uint16_t i = 0; i < screen_width; ++i) {
			for (uint16_t j = 0; j < screen_height; ++j) {
				nv_plot_pixel(i, j, m);
			}
		}
		++l;
		m = (l/2) << 16 | (l*3/4) << 8 | l;
	}
	for (uint16_t i = 0; i < screen_width; ++i) {
		for (uint16_t j = 0; j < screen_height; ++j) {
			nv_plot_pixel(i, j, COLOUR(0, 255, 0, 0));
		}
	}
}
