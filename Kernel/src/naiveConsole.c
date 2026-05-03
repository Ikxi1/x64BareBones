#include <naiveConsole.h>


static uint32 uintToBase(uint64 value, char * buffer, uint32 base);

static char buffer[64] = { '0' };
static uint8 *const videoBase = (uint8 *)0xB8000;
static uint8 *const videoMax =  (uint8 *)0xB8FA0;
// static uint8 *videoCurrent =    (uint8 *)0xB8000;
static const uint32 videoWidth = 80;
static const uint32 videoHeight = 25;
uint8 cursor_x = 0;
uint8 cursor_y = 0;


void nc_print_char(char character)
{
      uint8 *videoCurrent = videoBase + (cursor_x + videoWidth*cursor_y) * 2; // *2 because 16bit/character
      *videoCurrent++ = character;
      *videoCurrent = 0x02; // colour, black background 0, green text 2
      screen_text.ptr[cursor_x + videoWidth*cursor_y] = character;
      screen_text.count++;
      if (screen_text.count > (videoWidth * videoHeight))
      {
            screen_text.count = 0;
      }
      cursor_x++;
      if (cursor_x >= 80)
      {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= 25)
            {
                  cursor_y = 0;
            }
      }
}


void nc_delete_char()
{
      if (cursor_x > 0)
      {
            cursor_x--;
            uint8 *videoCurrent = videoBase + (cursor_x + videoWidth*cursor_y) * 2;
            *videoCurrent++ = 0;
            *videoCurrent = 0x02;

      }
}


void nc_print(const char * string, uint8 newline)
{
      for (int i = 0; string[i] != 0; i++) {
            nc_print_char(string[i]);
      }

      if (newline == 1) nc_newline();
}


void nc_newline()
{
      for (int i = cursor_x; i < 80; i++)
      {
            nc_print_char(' ');
      }
}


void ncPrintDec(uint64 value, uint8 newline) {
      ncPrintBase(value, 10, newline);
}


void ncPrintHex(uint64 value, uint8 newline) {
      ncPrintBase(value, 16, newline);
}


void ncPrintBin(uint64 value, uint8 newline) {
      ncPrintBase(value, 2, newline);
}


void ncPrintBase(uint64 value, uint32 base, uint8 newline) {
      uintToBase(value, buffer, base);
      nc_print(buffer, newline);
}


void nc_clear() {
      for (int i = 0; i < videoHeight * videoWidth; i++) {
            videoBase[i * 2] = ' ';
      }
      cursor_x = 0;
      cursor_y = 0;
}


static uint32 uintToBase(uint64 value, char * buffer, uint32 base) {
      char *p = buffer;
      char *p1, *p2;
      uint32 digits = 0;

      // Calculate characters for each digit
      do
      {
            uint32 remainder = value % base;
            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      }
      while (value /= base);

      // Terminate string in buffer.
      *p = 0;

      // Reverse string in buffer.
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


// void ncRainbow() {
//       // ncClear();
//       static unsigned short j = 0;
//       // int k = 0;
//       unsigned background_colour = 0 << 12;
//       unsigned foreground_colour = 2 << 8;
//       unsigned colour = background_colour | foreground_colour;
//       for (unsigned short i = 0; i < videoMax;)
//       {
//             videoBase[i] = colour | j % 0b0000000011111111;
//             i++;
//       }
//       j++;
// }


void nc_render_cursor()
{
      return;
}


void *get_text_ptr()
{
      return (void *)(screen_text.ptr + screen_text.count);
}


void init_console()
{
      screen_text.capacity = videoWidth * videoHeight;
      screen_text.count = 0;
      memset_char(screen_text.ptr, '\0', screen_text.capacity);
}
