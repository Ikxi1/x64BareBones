t#include <naiveKeyboard.h>
#include <naiveConsole.h>

uint32 key_lut[0xFF];

void init_LUT() {
    // replace a[ with key_lut[ later
    // this is just to make typing now easier

    // Normal key layout row by row
    //              ESC                   F1                   F2                   F3                   F4                   F5                   F6                   F7                   F8                   F9                  F10                  F11                  F12
    key_lut[1]   = 0x1B; key_lut[59]  = 0x00; key_lut[60]  = 0x00; key_lut[61]  = 0x00; key_lut[62]  = 0x00; key_lut[63]  = 0x00; key_lut[64]  = 0x00; key_lut[65]  = 0x00; key_lut[66]  = 0x00; key_lut[67]  = 0x00; key_lut[68]  = 0x00; key_lut[87]  = 0x00; key_lut[88]  = 0x00;
    //               \0                    ^                    1                    2                    3                    4                    5                    6                    7                    8                    9                    0                    ß                    ´            BACKSPACE
    key_lut[0]   = 0x00; key_lut[41]  = 0x5E; key_lut[2]   = 0x31; key_lut[3]   = 0x32; key_lut[4]   = 0x33; key_lut[5]   = 0x34; key_lut[6]   = 0x35; key_lut[7]   = 0x36; key_lut[8]   = 0x37; key_lut[9]   = 0x38; key_lut[10]  = 0x39; key_lut[11]  = 0x30; key_lut[12]  = 0xDF; key_lut[13]  = 0xB4; key_lut[14]  = 0x08;
    //             HTAB                                         q                    w                    e                    r                    t                    z                    u                    i                    o                    p                    ü                    +                ENTER
    key_lut[15]  = 0x09;                      key_lut[16]  = 0x71; key_lut[17]  = 0x77; key_lut[18]  = 0x65; key_lut[19]  = 0x72; key_lut[20]  = 0x74; key_lut[21]  = 0x7A; key_lut[22]  = 0x75; key_lut[23]  = 0x69; key_lut[24]  = 0x6F; key_lut[25]  = 0x70; key_lut[26]  = 0xFC; key_lut[27]  = 0x2B; key_lut[28]  = 0x0A;
    //            ENTER                                         a                    s                    d                    f                    g                    h                    j                    k                    l                    ö                    ä                    #
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
}

// extended keys
// 224 56 Alt Gr
// 224 29 RCONTROL
// 224 83 Delete

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

volatile uint8 key_ready = 0;
char kb_char = 0;

void keyb_irq() {
    uint32 sc = inportb(0x60);
    ncPrintBase(sc, 10, 1);

    // Only handle key presses (make codes)

    // if (!(sc & 0x80)) {
    //     uint8 code = sc & 0x7F;
    //     if (code < 128) {      // char val = keylut[code];
    //         if (val) {
    //             kb_char = val; // store pressed key
    //             key_ready = 1; // signal main loop
    //         }
    //     }
    // }
}
