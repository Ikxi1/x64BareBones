#include <naiveConsole.h>
#include <naiveKeyboard.h>
#include <naiveTimer.h>
#include <naiveVideo.h>
#include <naiveMemory.h>
#include <process.h>


extern uint8 endOfKernel;

void func1();
void func2();


int _main ()
{
      ncClear();

      ncPrint("HEEEEEEELP", 1);

      // start the two processes and then switch them in the while loop

      while (1)
      {
            uint64 start_time = timer_get();

            // ncPrintBase(start_time, 10, 1);

            build_key_event();
            if (key_event.key != 0)
            {
                  const char *c = (const char*)&key_event.key;
                  ncPrint((const char*)&key_event.key, 0);
                  // ncPrintBase(key_event.key, 10, 1);
                  key_event.key = 0;
            }

            // nv_rainbow();
            // ncRainbow();

            // key_event.key = 0;

            uint64 end_time = timer_get();
            while ((end_time - start_time) < 1000/60)
            {
                  __asm__("hlt");
                  end_time = timer_get();
            }
            if (end_time - start_time > 1000/60)
            {
                  ncPrint("Execution took more than 1/60th second.", 1);
            }
      }

      // endless loop for testing
      // while (1) {}

      return 0;
}


void func1 ()
{
      while (1)
      {
            ncPrint("func1", 0);
      }
}


void func2 ()
{
      while (1)
      {
            ncPrint("func2", 0);
      }
}
