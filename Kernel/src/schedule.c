#include <schedule.h>
#include <naiveConsole.h>



uint64 schedule(uint8 *rsp, Process *process_list)
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
