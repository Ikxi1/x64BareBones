#include <naiveConsole.h>
#include <naiveKeyboard.h>


int _main () {

    init_LUT();

    ncClear();

    ncPrint("HEEEEEEELP", 1);

    while (1) {
        build_key_event();
        if (key_event.key != 0) {
            // const char *c = (const char*)&key_event.key;
            ncPrint((const char*)&key_event.key, 0);
            // ncPrintBase(key_event.key, 10, 1);
            key_event.key = 0;
        }
    }

    return 0;
}
