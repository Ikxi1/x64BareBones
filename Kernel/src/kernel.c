#include <naiveConsole.h>
#include <naiveKeyboard.h>


int _main () {

    ncClear();

    ncPrint("HEEEEEEELP", 1);

    while (1) {
        build_key_event();
        if (key_event.key != 0) {
            // const char *c = (const char*)&key_event.key;
            // ncPrint(c, 0);
            ncPrintBase(key_event.key, 10, 1);
            // key_ready = 0;
        }
    }

    return 0;
}