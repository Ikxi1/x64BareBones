/*
 * vt.h
 *
 *  Created on: 29/09/2008
 *      Author: row
 */

#ifndef VT_H_
#define VT_H_

#include <stdint.h>
#include <page.h>

#define VT_COUNT 8
#define INBUFFER_SIZE (MEM_BLOCK/VT_COUNT)

#define SCR_WIDTH 80
#define SCR_HEIGHT 25
#define TAB 8
#define WHITE_TXT 0x07


/*Initializes the virtual terminals*/
void setupVTs(void);

/*Switches from the current virtual terminal to another*/
void vtSwitchActive(uint32 newVT);

/*Puts a character on the virtual terminal output*/
void vtPutchar(uint32 c, uint32 targetVT);

/*Gets a character from the virtual terminal input*/
uint32 vtReadImpl(void * buff, uint32 len);

/*Moves the hardware cursor in the virtual terminal*/
void movCursor(int32 count, uint32 targetVT);

/*Clears the virtual terminal*/
void vtClear(uint32 targetVT);

/*Tries to attach a process to a terminal*/
void attach(uint32 PID);

/*Detaches a process from a terminal*/
void dettach(uint32 PID);

/*Keyboard driver required functions*/
void decInCursor();
void incInCursor();
uint32 feedBuffer(uint32 c);
#endif /* VT_H_ */
