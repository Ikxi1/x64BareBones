#include <naiveKeyboard.h>
#include <naiveConsole.h>


static uint32 ptr; // testing

/* circular buffer for keyboard events */
CIRC_BUF kb_isr_buf = {
      .buffer = {0},
      .head = 0,
      .tail = 0,
      .length = CIRC_BUF_LEN
};

KEY_EVENT key_event = {
      .flags = 0,
      .key = 0
};

uint32 key_lut[0xFF] = {0};
uint32 shift_key_lut[0xFF] = {0};
uint32 alt_gr_key_lut[0xFF] = {0};
uint32 released_key_lut[0xFF] = {0};


void init_LUT() {
      ptr = 0;
      // pressed
      // Normal key layout row by row
      //              ESC                   F1                   F2                   F3                   F4                   F5                   F6                   F7                   F8                   F9                  F10                  F11                  F12
      key_lut[1]   = 0x1B; key_lut[59]  = 0x00; key_lut[60]  = 0x00; key_lut[61]  = 0x00; key_lut[62]  = 0x00; key_lut[63]  = 0x00; key_lut[64]  = 0x00; key_lut[65]  = 0x00; key_lut[66]  = 0x00; key_lut[67]  = 0x00; key_lut[68]  = 0x00; key_lut[87]  = 0x00; key_lut[88]  = 0x00;
      //               \0                    ^                    1                    2                    3                    4                    5                    6                    7                    8                    9                    0                    ß                    ´            BACKSPACE
      key_lut[0]   = 0x00; key_lut[41]  = 0x5E; key_lut[2]   = 0x31; key_lut[3]   = 0x32; key_lut[4]   = 0x33; key_lut[5]   = 0x34; key_lut[6]   = 0x35; key_lut[7]   = 0x36; key_lut[8]   = 0x37; key_lut[9]   = 0x38; key_lut[10]  = 0x39; key_lut[11]  = 0x30; key_lut[12]  = 0xDF; key_lut[13]  = 0xB4; key_lut[14]  = 0x08;
      //             HTAB                                         q                    w                    e                    r                    t                    z                    u                    i                    o                    p                    ü                    +                ENTER
      key_lut[15]  = 0x09;                      key_lut[16]  = 0x71; key_lut[17]  = 0x77; key_lut[18]  = 0x65; key_lut[19]  = 0x72; key_lut[20]  = 0x74; key_lut[21]  = 0x7A; key_lut[22]  = 0x75; key_lut[23]  = 0x69; key_lut[24]  = 0x6F; key_lut[25]  = 0x70; key_lut[26]  = 0xFC; key_lut[27]  = 0x2B; key_lut[28]  = 0x0A;
      //             CAPS                                         a                    s                    d                    f                    g                    h                    j                    k                    l                    ö                    ä                    #
      key_lut[58]  = 0x00;                      key_lut[30]  = 0x61; key_lut[31]  = 0x73; key_lut[32]  = 0x64; key_lut[33]  = 0x66; key_lut[34]  = 0x67; key_lut[35]  = 0x68; key_lut[36]  = 0x6A; key_lut[37]  = 0x6B; key_lut[38]  = 0x6C; key_lut[39]  = 0xF6; key_lut[40]  = 0xE4; key_lut[43]  = 0x23;
      //           LSHIFT                    <                    y                    x                    c                    v                    b                    n                    m                    ,                    .                    -                                                         RSHIFT
      key_lut[42]  = 0x00; key_lut[86]  = 0x3C; key_lut[44]  = 0x79; key_lut[45]  = 0x78; key_lut[46]  = 0x63; key_lut[47]  = 0x76; key_lut[48]  = 0x62; key_lut[49]  = 0x6E; key_lut[50]  = 0x6D; key_lut[51]  = 0x2C; key_lut[52]  = 0x2E; key_lut[53]  = 0x2D;                                           key_lut[54]  = 0x00;
      //         LCONTROL                 LALT                SPACE
      key_lut[29]  = 0x00; key_lut[56]  = 0x00; key_lut[57]  = 0x20;

      //           NUMPAD                    /                    *
      key_lut[69]  = 0x00; key_lut[53]  = 0x2F; key_lut[55]  = 0x2A;
      //                7                    8                    9
      key_lut[71]  = 0x37; key_lut[72]  = 0x38; key_lut[73]  = 0x39;
      //                4                    5                    6
      key_lut[75]  = 0x34, key_lut[76]  = 0x35; key_lut[77]  = 0x36;
      //                1                    2                    3
      key_lut[79]  = 0x31; key_lut[80]  = 0x32; key_lut[81]  = 0x33;
      //                0                                         ,
      key_lut[82]  = 0x30;                      key_lut[83]  = 0x2C;

      // Shift key layout row by row
      //                      °                          !                          "                          $                          $                          %                          &                          /                          (                          )                          =                          ?                          `
      shift_key_lut[41]  = 0xB0; shift_key_lut[2]   = 0x33; shift_key_lut[3]   = 0x34; shift_key_lut[4]   = 0xA7; shift_key_lut[5]   = 0x24; shift_key_lut[6]   = 0x37; shift_key_lut[7]   = 0x26; shift_key_lut[8]   = 0x47; shift_key_lut[9]   = 0x40; shift_key_lut[10]  = 0x41; shift_key_lut[11]  = 0x3D; shift_key_lut[12]  = 0x3F; shift_key_lut[13]  = 0x60;
      //                      Q                          W                          E                          R                          T                          Z                          U                          I                          O                          P                          Ü                          *
      shift_key_lut[16]  = 0x51; shift_key_lut[17]  = 0x57; shift_key_lut[18]  = 0x45; shift_key_lut[19]  = 0x52; shift_key_lut[20]  = 0x54; shift_key_lut[21]  = 0x5A; shift_key_lut[22]  = 0x55; shift_key_lut[23]  = 0x59; shift_key_lut[24]  = 0x4F; shift_key_lut[25]  = 0x50; shift_key_lut[26]  = 0xDC; shift_key_lut[27]  = 0x2A;
      //                      A                          S                          D                          F                          G                          H                          J                          K                          L                          Ö                          Ä                          '
      shift_key_lut[30]  = 0x41; shift_key_lut[31]  = 0x53; shift_key_lut[32]  = 0x44; shift_key_lut[33]  = 0x46; shift_key_lut[34]  = 0x47; shift_key_lut[35]  = 0x48; shift_key_lut[36]  = 0x4A; shift_key_lut[37]  = 0x4B; shift_key_lut[38]  = 0x4C; shift_key_lut[39]  = 0xD6; shift_key_lut[40]  = 0xC4; shift_key_lut[43]  = 0x27;
      //                      <                          Y                          X                          C                          V                          B                          N                          M                          ;                          :                          _
      shift_key_lut[86]  = 0x3E; shift_key_lut[44]  = 0x59; shift_key_lut[45]  = 0x58; shift_key_lut[46]  = 0x43; shift_key_lut[47]  = 0x56; shift_key_lut[48]  = 0x42; shift_key_lut[49]  = 0x4E; shift_key_lut[50]  = 0x4D; shift_key_lut[51]  = 0x3B; shift_key_lut[52]  = 0x3A; shift_key_lut[53]  = 0x5F;

      // Alt Gr key layout not row by row
      //                       {                           [                           ]                           }                           |                           \
      alt_gr_key_lut[8]   = 0x7B; alt_gr_key_lut[9]   = 0x5B; alt_gr_key_lut[10]  = 0x5D; alt_gr_key_lut[11]  = 0x7D; alt_gr_key_lut[86]  = 0x7C; alt_gr_key_lut[12]  = 0x5C;
      //                       ~                           @                           µ                             €
      alt_gr_key_lut[27]  = 0x7E; alt_gr_key_lut[16]  = 0x40; alt_gr_key_lut[50]  = 0xB5; alt_gr_key_lut[18]  = 0x20AC;



      // release keys
      // Normal key layout row by row
      //                                                F1                         F2                         F3                         F4                         F5                         F6                         F7                         F8                         F9                        F10                        F11                        F12
                                 released_key_lut[59]  = 1; released_key_lut[188] = 1; released_key_lut[189] = 1; released_key_lut[190] = 1; released_key_lut[191] = 1; released_key_lut[192] = 1; released_key_lut[193] = 1; released_key_lut[194] = 1; released_key_lut[195] = 1; released_key_lut[196] = 1; released_key_lut[215] = 1; released_key_lut[216] = 1;
      //                     \0                          ^                          1                          2                          3                          4                          5                          6                          7                          8                          9                          0                          ß                          ´
      released_key_lut[0]   = 1; released_key_lut[169] = 1; released_key_lut[130] = 1; released_key_lut[131] = 1; released_key_lut[132] = 1; released_key_lut[133] = 1; released_key_lut[134] = 1; released_key_lut[135] = 1; released_key_lut[136] = 1; released_key_lut[137] = 1; released_key_lut[138] = 1; released_key_lut[139] = 1; released_key_lut[140] = 1; released_key_lut[141] = 1;
      //                   HTAB                                                     q                          w                          e                          r                          t                          z                          u                          i                          o                          p                          ü                          +
      released_key_lut[143] = 1;                            released_key_lut[144] = 1; released_key_lut[145] = 1; released_key_lut[146] = 1; released_key_lut[147] = 1; released_key_lut[148] = 1; released_key_lut[149] = 1; released_key_lut[150] = 1; released_key_lut[151] = 1; released_key_lut[152] = 1; released_key_lut[153] = 1; released_key_lut[154] = 1; released_key_lut[155] = 1;
      //                                                                            a                          s                          d                          f                          g                          h                          j                          k                          l                          ö                          ä                          #
                                                            released_key_lut[158] = 1; released_key_lut[159] = 1; released_key_lut[160] = 1; released_key_lut[161] = 1; released_key_lut[162] = 1; released_key_lut[163] = 1; released_key_lut[164] = 1; released_key_lut[165] = 1; released_key_lut[166] = 1; released_key_lut[167] = 1; released_key_lut[168] = 1; released_key_lut[171] = 1;
      //                                                 <                          y                          x                          c                          v                          b                          n                          m                          ,                          .                          -
                                 released_key_lut[214] = 1; released_key_lut[172] = 1; released_key_lut[173] = 1; released_key_lut[174] = 1; released_key_lut[175] = 1; released_key_lut[176] = 1; released_key_lut[177] = 1; released_key_lut[178] = 1; released_key_lut[179] = 1; released_key_lut[180] = 1; released_key_lut[181] = 1;
      //                                            2                            SPACE
                                                            released_key_lut[185] = 1;

      //                 NUMPAD                                                     *
      released_key_lut[197] = 1;                            released_key_lut[183] = 1;
      //                      7                          8                          9
      released_key_lut[199] = 1; released_key_lut[200] = 1; released_key_lut[201] = 1;
      //                      4                          5                          6
      released_key_lut[203] = 1, released_key_lut[204] = 1; released_key_lut[205] = 1;
      //                      1                          2                          3
      released_key_lut[207] = 1; released_key_lut[208] = 1; released_key_lut[209] = 1;
      //                      0                                                     ,
      released_key_lut[210] = 1;                            released_key_lut[211] = 1;
}

// extended keys
// 224 56 Alt Gr
// 224 29 RCONTROL
// 224 83 Delete
// 224 53 - 224 181 Numpad /

// very extended keys (only on old samsung laptop)
// my modern desktop has those without the fake shift
// it's a fake extended shift (42) first and then the actual key press, so just ignore the fake extended 42
// 224 42 224 91 windows key
// 224 42 224 93 context menu (right click)
// 224 42 224 72 arrow_up
// 224 42 224 80 arrow_down
// 224 42 224 75 arrow_left
// 224 42 224 77 arrow_right
// 224 42 224 83 delete


void keyb_irq()
{
      uint32 sc = inportb(0x60);
      // ncPrintBase(sc, 10, 1);

      // put scancode into circular buffer to be read OUTSIDE the interrupt
      uint8 next = kb_isr_buf.head + 1;
      if (next == kb_isr_buf.tail) return; // buffer full, discard data
      if (next >= kb_isr_buf.length) next = 0; // point head to beginning
      kb_isr_buf.buffer[kb_isr_buf.head] = sc;
      kb_isr_buf.head = next;
}


void build_key_event()
{
      uint8 next;

      if (kb_isr_buf.head == kb_isr_buf.tail) return; // no data available
      next = kb_isr_buf.tail + 1;
      // at end of buf, point tail to beginning
      if (next >= kb_isr_buf.length) next = 0;

      // put data into KEY_EVENT
      uint32 c = kb_isr_buf.buffer[kb_isr_buf.tail];

      if (key_lut[c] != 0) {
            key_event.key = key_lut[c];
      }
      else if (released_key_lut[c] != 0) {
            key_event.key = 0;
      }

      kb_isr_buf.tail = next;
}


void build_key_event_old() {
      uint8 next;
      if (kb_isr_buf.head == kb_isr_buf.tail) return; // no data available
      next = kb_isr_buf.tail + 1;
      if (next >= kb_isr_buf.length) next = 0; // point tail to beginning

      // put data into KEY_EVENT
      uint32 c = kb_isr_buf.buffer[kb_isr_buf.tail];
      // first check, if D/EXTENDED flag is set
      if (key_event.flags & KEY_DEXTENDED) {

      }
      else if (key_event.flags & KEY_EXTENDED) {

      }
      else if (key_event.flags & KEY_SHIFT) {
            if (key_lut[c] != 0) {
                  key_event.key = shift_key_lut[c];
            }
      }
      // no flags set
      else {
            // check for char
            if (key_lut[c] != 0) {
                  key_event.key = key_lut[c];
            }
            else if (released_key_lut[c] != 0) {
                  key_event.key = 0;
            }
            else if (c == 42 || c == 54) {
                  key_event.flags |= KEY_SHIFT;
            }
            else if (c == 56) {
                  key_event.flags |= KEY_ALT;
            }
            else if (c == 29) {
                  key_event.flags |= KEY_CTRL;
            }
            else if (c == 58) {
                  key_event.flags ^= KEY_CAPS;
            }
            else if (c == 224) {
                  key_event.flags |= KEY_EXTENDED;
            }
      }
      // ALT GR, RSHIFT

      kb_isr_buf.tail = next;
}
