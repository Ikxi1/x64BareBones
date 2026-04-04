#include <schedule.h>
#include <naiveConsole.h>
#include <process.h>


uint64 schedule(uint8 *rsp)
{
      static bool b = false;
      if (b == false)
      {
            b = true;
            return (uint64)process_list[0].rsp;
      }
      else
      {
            b = false;
            return (uint64)process_list[1].rsp;
      }
}
