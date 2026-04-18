#include <schedule.h>
#include <naiveConsole.h>
#include <process.h>


uint8 *schedule(uint8 *rsp, Process *process_list)
{
      process_list[currPID].rsp = rsp;

      currPID++;

      while (process_list[currPID].stack == 0)
      {
            currPID++;
      }

      return process_list[currPID].rsp;
}
