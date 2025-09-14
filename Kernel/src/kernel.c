#include <interruptHandler.h>
#include <naiveConsole.h>


int _main () {

    setupInterrupts();

    _Sti();

    ncClear();

    ncPrint("HEEEEEEELP", 1);

    while (1) {}

    return 0;
}