#include <schedule.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <process.h>


void *schedule()
{
      static bool b = false;
      if (b == false)
      {
            b = true;
            return &process_list[0].rsp;
      }
      else
      {
            b = false;
            return &process_list[1].rsp;
      }
}
