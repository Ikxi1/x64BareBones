#include <naiveConsole.h>
#include <naiveKeyboard.h>


int _main () {

    ncClear();

    ncPrint("HEEEEEEELP", 1);

    while (1) {
        if (key_ready == 1) {
            const char *c = &kb_char;
            ncPrint(c, 0);
            key_ready = 0;
        }
    }

    return 0;
}