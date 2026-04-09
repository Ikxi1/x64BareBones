#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>
#include <process.h>

uint8 currPID = 0;

/* schedule

 Changes from one process to another.
 Saves current stack pointer. // not right now
 Returns stack pointer for next process.
*/
uint8 *schedule(uint8 *rsp, Process *process_list);

#endif // SCHEDULE_H
