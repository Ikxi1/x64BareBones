#include <schedule.h>
#include <stdint.h>
#include <naiveConsole.h>


void *schedule()
{
      static bool b = false;
      if (b == false)
      {
            b = true;
            return *func1;
      }
      else if (b == true) {
            b = false;
      }
      return *func2;
}


void func1 ()
{
      while (1) {
            ncPrint("func1", 0);
      }
}


void func2 ()
{
      while (1) {
            ncPrint("func2", 0);
      }
}
