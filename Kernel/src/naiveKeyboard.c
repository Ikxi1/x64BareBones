#include <naiveKeyboard.h>
#include <naiveConsole.h>

uint32 key_lut[0xFF];

void init_LUT() {
    // replace a[ with key_lut[ later
    // this is just to make typing now easier

    // Normal key layout row by row
    //         \0             \0              1              2              3              4              5              6              7              8              9              0              ß              ´      BACKSPACE
    a[0]   = 0x00; a[1]   = 0x00; a[2]   = 0x31; a[3]   = 0x32; a[4]   = 0x33; a[5]   = 0x34; a[6]   = 0x35; a[7]   = 0x36; a[8]   = 0x37; a[9]   = 0x38; a[10]  = 0x39; a[11]  = 0x30; a[12]  = 0xDF; a[13]  = 0xB4; a[14]  = 0x08;
    //       HTAB                             q              w              e              r              t              z              u              i              o              p              ü              +                         ENTER
    a[15]  = 0x09;                a[16]  = 0x71; a[17]  = 0x77; a[18]  = 0x65; a[19]  = 0x72; a[20]  = 0x74; a[21]  = 0x7A; a[22]  = 0x75; a[23]  = 0x69; a[24]  = 0x6F; a[25]  = 0x70; a[26]  = 0xFC; a[27]  = 0x2B;                a[28]  = 0x0A;
    //      ENTER                             a              s              d              f              g              h              j              k              l              ö              ä              #
    a[58]  = 0x00;                a[30]  = 0x61; a[31]  = 0x73; a[32]  = 0x64; a[33]  = 0x66; a[34]  = 0x67; a[35]  = 0x68; a[36]  = 0x6A; a[37]  = 0x6B; a[38]  = 0x6C; a[39]  = 0xF6; a[40]  = 0xE4; a[43]  = 0x23;
    //     LSHIFT              <              y              x              c              v              b              n              m              ,              .              -                                       RSHIFT
    a[42]  = 0x00; a[86]  = 0x3C; a[44]  = 0x79; a[45]  = 0x78; a[46]  = 0x63; a[47]  = 0x76; a[48]  = 0x62; a[49]  = 0x6E; a[50]  = 0x6D; a[51]  = 0x2C; a[52]  = 0x2E; a[53]  = 0x2D; a[54]                                = 0x00;
    //   LCONTROL
    a[29]  = 0x00;
}

volatile uint8 key_ready = 0;
char kb_char = 0;

void keyb_irq() {
    uint32 sc = inportb(0x60);
    ncPrintBase(sc, 10, 1);

    // Only handle key presses (make codes)

    // if (!(sc & 0x80)) {
    //     uint8 code = sc & 0x7F;
    //     if (code < 128) {    //         char val = keylut[code];
    //         if (val) {
    //             kb_char = val; // store pressed key
    //             key_ready = 1; // signal main loop
    //         }
    //     }
    // }
}
