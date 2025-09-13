/*
 * vt.c
 *
 *  Created on: 29/09/2008
 *      Author: row
 */

#include <vt.h>
#include <page.h>
#include <keyboard.h>
#include <scheduler.h>
#include <process.h>
#include <syscalls.h>
#include <semaphore.h>
#include <shield.h>

#include <vtprintf.h>

typedef struct
{
	uint32 attachedPID;
	/*output members*/
	char * outBuffer;
	uint32 outCursor;
	/*input members*/
	char * inBuffer;
	uint32 inStart;		/*< Start of the input buffer*/
	uint32 inEnd;		/*< End of the input buffer*/
	uint32 inCursor;	/*< Input cursor where we insert or replace characters*/
	uint32 scrCursor;	/*< Where we start showing the buffer in the screen*/
}vt;

typedef struct
{
	uint8 charac;
	uint8 attrib;
}cell;

#define OUT_WIDTH SCR_WIDTH
#define OUT_HEIGHT (SCR_HEIGHT-1)
#define IN_START (OUT_WIDTH*OUT_HEIGHT)
#define IN_WIDTH SCR_WIDTH

#define PREV(x, len) ((x-1+len) % len)
#define NEXT(x, len) ((x+1) % len)

#ifdef TESTING
	void movecursor(int pos) {}
	/*Should be allocated (and printed?) by the tester*/
	cell * video = 0;
#else
	/*Defined in dbg_video.asm, moves the hardware cursor*/
	void movecursor(int pos);
	static cell * video = (cell*) 0xB8000;
#endif

static void newline(uint32 targetVT);
static void tab(uint32 targetVT);
static void putOtherChar(char c, uint32 targetVT);
static void scrollVT(uint32 targetVT);
static uint32 buffLen(int32 end, int32 start);
static uint32 addChar(uint32 vt, uint32 c);
static void reprintInputBuffer(uint32 vt);

static vt terminals[VT_COUNT];

static uint32 currentVT;

void setupVTs(void)
{
	/* Enough space for a maximum of 8 terminals */
	/* it should be changed for protected page */
	void * vtOutMem = page();
	void * vtInMem = page();
	uint32 i = 0;
	currentVT = 0;

	/* Initialize the keyboard*/
	initKeyboard();

	/*Clear the terminals*/
	for (i=0; i<MEM_BLOCK; i++)
		((char*)vtOutMem)[i] = ' ';

	/* Initialize each terminal */
	for (i=0; i<VT_COUNT; i++)
	{
		/*Setup terminal output*/
		terminals[i].outBuffer = vtOutMem;
		terminals[i].outCursor = 0;
		terminals[i].attachedPID = 0;
		vtOutMem += OUT_WIDTH * (OUT_HEIGHT);
		vtClear(i);
		/*Setup terminal input*/
		terminals[i].inBuffer = vtInMem;
		terminals[i].inStart = 0;
		terminals[i].inEnd = 1;
		terminals[i].inCursor = 0;
		terminals[i].scrCursor = 0;
		vtInMem += INBUFFER_SIZE;
		/*Increment the semaphore that indicates if a process can attach*/
		semaphoreInc(-(VT_COUNT+i+1));
	}
	movecursor(IN_START);
}

void vtSwitchActive(uint32 newVT)
{
	uint32 i;

	if (newVT >= VT_COUNT)
		return;

//	cprintf("%d and %d\n",terminals[currentVT].inCursor,terminals[currentVT].scrCursor);

	currentVT = newVT;

	/*Reprint the buffer (in fact this will fill it with spaces)*/
	reprintInputBuffer(currentVT);

	/*Move the hardware cursor to the inCursor relative to the scrCursor*/
	movecursor(IN_START +
			terminals[currentVT].inCursor - terminals[currentVT].scrCursor);

	for (i=0; i<OUT_WIDTH*(OUT_HEIGHT); i++)
	{
		video[i].attrib = WHITE_TXT;
		video[i].charac = terminals[currentVT].outBuffer[i];
	}
}

void vtPutchar(uint32 targetVT, uint32 c)
{
	switch (c)
	{
	case '\n':
		newline(targetVT); break;
	case '\t':
		tab(targetVT); break;
	case ' ':
		movCursor(1, targetVT); break;
	case '\b':
		movCursor(-1, targetVT); break;
		putOtherChar(' ', targetVT);
		movCursor(-1, targetVT); break;
	default:
		putOtherChar(c, targetVT); break;
	}
}

void attach(uint32 PID)
{
	/*The process will block if there is another process attached*/
	uint32 vt = getProcess(PID)->vt;

	/*Reset the input*/
	semaphoreDec(-(VT_COUNT+vt+1), PID);
	terminals[vt].attachedPID = PID;
	terminals[vt].inStart = 0;
	terminals[vt].inCursor= 0;
	terminals[vt].inEnd = 1;
	while(semaphoreNBDec(-(vt+1), 0) == TRUE);
}

void dettach(uint32 PID)
{
	uint32 vt = getProcess(PID)->vt;

	if (PID == terminals[vt].attachedPID)
	{
		terminals[vt].attachedPID = 0;
		semaphoreInc(-(VT_COUNT+vt+1));
	}
}

void movCursor(int32 count, uint32 targetVT)
{
	if (count < 0 && -count > terminals[targetVT].outCursor)
		terminals[targetVT].outCursor = 0;
	else
		terminals[targetVT].outCursor += count;

	if (terminals[targetVT].outCursor >= (OUT_HEIGHT)*OUT_WIDTH)
	{
		terminals[targetVT].outCursor -= OUT_WIDTH;
		scrollVT(targetVT);
	}
}

void vtClear(uint32 targetVT)
{
	int i;

	terminals[targetVT].outCursor = 0;
	for (i=0; i<OUT_WIDTH*(OUT_HEIGHT); i++)
		terminals[targetVT].outBuffer[i] = ' ';

	if (targetVT == currentVT)
	{
		for (i=0; i<OUT_WIDTH*(OUT_HEIGHT); i++)
		{
			video[i].charac = ' ';
			video[i].attrib = WHITE_TXT;
		}
	}
}

uint32 vtGetAttachedProcess(uint32 targetVT)
{
	if (targetVT >= VT_COUNT)
		return 0;
	return terminals[targetVT].attachedPID;
}

static void newline(uint32 targetVT)
{
	movCursor(OUT_WIDTH - terminals[targetVT].outCursor % OUT_WIDTH, targetVT);
}

static void tab(uint32 targetVT)
{
	int k;

	k = TAB - terminals[targetVT].outCursor % TAB;
	if (k == 0)
		k = TAB;

	movCursor(k, targetVT);
}

static void putOtherChar(char c, uint32 targetVT)
{
	terminals[targetVT].outBuffer[terminals[targetVT].outCursor] = c;
	if (targetVT == currentVT)
	{
		video[terminals[targetVT].outCursor].charac = c;
		video[terminals[targetVT].outCursor].attrib = WHITE_TXT;
	}
	movCursor(1, targetVT);
}

static void scrollVT(uint32 targetVT)
{
	int i;

	for (i=0; i<OUT_WIDTH*(OUT_HEIGHT-1); i++)
	{
		terminals[targetVT].outBuffer[i] =
		terminals[targetVT].outBuffer[OUT_WIDTH + i];
	}

	for (i=0; i<OUT_WIDTH; i++)
		terminals[targetVT].outBuffer[terminals[targetVT].outCursor++] = ' ';

	terminals[targetVT].outCursor -= OUT_WIDTH;

	if (targetVT == currentVT)
	{
		for (i=0; i<OUT_WIDTH*(OUT_HEIGHT-1); i++)
		{
			video[i].attrib = video[OUT_WIDTH + i].attrib;
			video[i].charac = video[OUT_WIDTH + i].charac;
		}

		for (i=OUT_WIDTH*(OUT_HEIGHT-1); i<OUT_WIDTH*(OUT_HEIGHT); i++)
		{
			video[i].attrib = WHITE_TXT;
			video[i].charac = ' ';
		}
	}
}

/*Keyboard driver required functions*/
void decInCursor()
{
	uint32 index = (terminals[currentVT].inStart +
			terminals[currentVT].inCursor) % INBUFFER_SIZE;

	if (terminals[currentVT].inCursor > 0 &&
		terminals[currentVT].inBuffer[PREV(index, INBUFFER_SIZE)] != '\n')
	{
		/*move the screen display cursor if necessary*/
		if (terminals[currentVT].inCursor == terminals[currentVT].scrCursor)
		{
			terminals[currentVT].scrCursor--;
			reprintInputBuffer(currentVT);
		}

		/*move the input cursor*/
		terminals[currentVT].inCursor--;

		/*move the hardware cursor*/
		movecursor(IN_START +
			terminals[currentVT].inCursor - terminals[currentVT].scrCursor);
	}
}

void incInCursor()
{
	uint32 len = buffLen(terminals[currentVT].inEnd,
			terminals[currentVT].inStart);

	if (terminals[currentVT].inCursor < len)
	{
		/*move the screen display cursor if necessary*/
		if (terminals[currentVT].inCursor ==
			terminals[currentVT].scrCursor + IN_WIDTH - 1)
		{
			terminals[currentVT].scrCursor++;
			reprintInputBuffer(currentVT);
		}

		terminals[currentVT].inCursor++;

		/*move the hardware cursor*/
		movecursor(IN_START +
			terminals[currentVT].inCursor - terminals[currentVT].scrCursor);
	}
}

uint32 feedBuffer(uint32 c)
{
	int32 i, prev;
	uint32 len = buffLen(terminals[currentVT].inEnd,
			terminals[currentVT].inStart);
	switch (c)
	{
	case '\n':
		/*Check if there is space in the buffer*/
		if (len == INBUFFER_SIZE)
			return FALSE;

		/* Increment the semaphore that blocks the process attached to input
		 * the loop goes from the last character, to the previous '\n' the
		 * beginning of the buffer, the increment at the beginning is for the
		 * '\n' we're feeding */
		semaphoreInc(-(currentVT+1));
		for (i=PREV(terminals[currentVT].inEnd, INBUFFER_SIZE);
			i != terminals[currentVT].inStart &&
			terminals[currentVT].inBuffer[i] != '\n';
			i = PREV(i, INBUFFER_SIZE))
			semaphoreInc(-(currentVT+1));

		/* Move inCursor to the end of the buffer to append correctly the '\n'
		 * move also scrCursor just after the \n. addChar should never fail in
		 * this case!*/
		prev = terminals[currentVT].inCursor;
		addChar(currentVT, '\n');
		//terminals[currentVT].scrCursor = len+1;
		terminals[currentVT].inCursor = len;
		terminals[currentVT].scrCursor = len+1;

		/*Reprint the buffer (in fact this will fill it with spaces)*/
		reprintInputBuffer(currentVT);
		//cprintf("Current term %d\n",currentVT);
		/** TODO Ver reprint? */
		terminals[currentVT].inCursor = prev+1;
		terminals[currentVT].scrCursor = 0;

		/*render the input buffer*/
		movecursor(IN_START +
			terminals[currentVT].inCursor - terminals[currentVT].scrCursor);
		//terminals[currentVT].scrCursor = terminals[currentVT].inCursor = 0;
		//cprintf("%d and %d\n",terminals[currentVT].inCursor,terminals[currentVT].scrCursor);
		//movecursor(IN_START);

		return TRUE;
	case '\b':
		/*Get the current index in the buffer*/
		i = (terminals[currentVT].inStart + terminals[currentVT].inCursor)
			% INBUFFER_SIZE;

		/*Get the index  of the character to be overwritten (prev)*/
		prev = PREV(i, INBUFFER_SIZE);

		/*Check that we can actually delete something*/
		if (terminals[currentVT].inCursor == 0 ||
				terminals[currentVT].inBuffer[PREV(i, INBUFFER_SIZE)] == '\n')
			return FALSE;

		/*Shift the buffer*/
		for (; i != terminals[currentVT].inEnd; i = NEXT(i, INBUFFER_SIZE))
		{
			/*Overwrite the previous with the current*/
			terminals[currentVT].inBuffer[prev] =
				terminals[currentVT].inBuffer[i];

			/*Update the previous*/
			prev=i;
		}

		/*Decrement the end of the buffer*/
		terminals[currentVT].inEnd =
			PREV(terminals[currentVT].inEnd, INBUFFER_SIZE);

		/*Decrement the scr cursor if necessary*/
		if (terminals[currentVT].scrCursor == terminals[currentVT].inCursor)
			terminals[currentVT].scrCursor--;

		/*Decrement the input cursor*/
		terminals[currentVT].inCursor--;

		/*render the input buffer*/
		reprintInputBuffer(currentVT);

		/*move the hardware cursor*/
		movecursor(IN_START +
			terminals[currentVT].inCursor - terminals[currentVT].scrCursor);

		return TRUE;
	default:
		return addChar(currentVT, c);
	}
}

uint32 vtReadImpl(void * buff, uint32 len)
{
	uint32 vt = getProcess(schedGetCurrentProcess()->PID)->vt;
	uint32  iniLen = len;
	char * cbuff = (char*)buff;

	/*Check if calling process is attached to vt input, if not, try to attach!*/
	if (schedGetCurrentProcess()->PID != terminals[vt].attachedPID)
		attach(schedGetCurrentProcess()->PID);

	if (terminals[vt].inStart == PREV(terminals[vt].inEnd,INBUFFER_SIZE))
	{
		/*Lock until we can read a character from the buffer*/
		semaphoreDec(-(vt+1), schedGetCurrentProcess()->PID);
		cbuff[iniLen - len] = terminals[vt].inBuffer[terminals[vt].inStart];
		terminals[vt].inStart = NEXT(terminals[vt].inStart, INBUFFER_SIZE);
		len--;
	}

	/*Read len characters from the buffer (or less if the buffer is shorter)*/
	for (; terminals[vt].inStart != PREV(terminals[vt].inEnd,INBUFFER_SIZE) && len; len--)
	{
		cbuff[iniLen - len] = terminals[vt].inBuffer[terminals[vt].inStart];
		terminals[vt].inStart = NEXT(terminals[vt].inStart, INBUFFER_SIZE);
		semaphoreNBDec(-(vt+1), schedGetCurrentProcess()->PID);
	}

	terminals[vt].inCursor -= (iniLen - len);

	return iniLen - len;
}

static uint32 buffLen(int32 end, int32 start)
{
	return (end - start > 0)? end - start : INBUFFER_SIZE - (start - end);
}

static uint32 addChar(uint32 vt, uint32 c)
{
	uint32 i, start;
	char shft;

	/* Check if the buffer is full, the -1 is there to avoid the situation in
	 * which the buffer is full and the user can't enter a \n */
	if (buffLen(terminals[vt].inEnd, terminals[vt].inStart) >= INBUFFER_SIZE-1)
		return FALSE;

	/*Get the index in which we are inserting the character*/
	start = (terminals[vt].inStart + terminals[vt].inCursor) % INBUFFER_SIZE;

	/*Shift the buffer*/
	for (i=start; i != terminals[vt].inEnd; i = NEXT(i, INBUFFER_SIZE))
	{
		/*Save the first character we will shift right*/
		shft = terminals[vt].inBuffer[i];

		/*Set the character we're inserting*/
		terminals[vt].inBuffer[i] = c;

		/*Set the next character to be inserted*/
		c = shft;
	}

	/*Put the last character*/
	terminals[vt].inBuffer[i] = c;

	/*Grow the buffer*/
	terminals[vt].inEnd = NEXT(terminals[vt].inEnd, INBUFFER_SIZE);

	/*Increment the inCursor*/
	terminals[vt].inCursor++;

	/*Redraw the buffer if necessary or simply add the character*/
	if (terminals[vt].inCursor - terminals[vt].scrCursor > IN_WIDTH - 1)
		terminals[vt].scrCursor++;

	reprintInputBuffer(vt);

	movecursor(IN_START +
		terminals[currentVT].inCursor - terminals[currentVT].scrCursor);

	return TRUE;
}

static void reprintInputBuffer(uint32 vt)
{
	uint32 c = (terminals[vt].inStart + terminals[vt].scrCursor) % INBUFFER_SIZE;
	uint32 i;

	/**/
	for (i=IN_START; c != terminals[vt].inEnd && i<IN_START+IN_WIDTH;
		i++, c = NEXT(c, INBUFFER_SIZE))
		video[i].charac = terminals[vt].inBuffer[c];

	for (; i < IN_START+IN_WIDTH; i++)
		video[i].charac = ' ';
}
