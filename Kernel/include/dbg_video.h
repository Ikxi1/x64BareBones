/*Debug Output Interface*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define SCR_WIDTH 80
#define SCR_HEIGHT 25
#define TAB 8

#define WHITE_TXT 0x07

#include <stdint.h>

typedef struct
{
	uint32 x, y;
	uint32 width,height;
	uint32 curx, cury;
}fbT;

/******************************/
/* Public function prototypes */
/******************************/
uint8 * getVideo();

void dbg_setFrame(fbT * f);

fbT * dbg_getFrame();

void dbg_clear();

void dbg_putchar(int c);

void dbg_printf(char * fmtStr, ...);

#endif
