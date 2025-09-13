#include <vt.h>
#include <vtprintf.h>

/*Prints a num in hex format on a virtual terminal*/
static void printhex(uint32 vt, uint32 hex);
static void printhexw(uint32 vt, uint32 hex, uint32 count);

/*Prints num on the current framebuffer*/
static void printnum(uint32 vt, uint32 num);

/*Prints str on the current framebuffer*/
static void printstr(uint32 vt, char * str);

/*Prints a natural number on the current framebuffer*/
static void printnat(uint32 vt, uint32 num);

void kprintf(char * fmtStr, ...)
{
	vtprintfp(VT_COUNT-1, fmtStr, ((uint32*)&fmtStr) + 1);
}

void vtprintf(uint32 vt, char * fmtStr, ...)
{
	vtprintfp(vt, fmtStr, ((uint32*)&fmtStr) + 1);
}

void vtprintfp(uint32 vt, char * fmtStr, void * vaArg)
{
	int i=0;

	while(fmtStr[i]!=0)
	{
		if (fmtStr[i] == '%')
		{
			switch (fmtStr[++i])
			{
			case 'd':
				printnum(vt, *(int32*)vaArg);
				break;
			case 's':
				printstr(vt, *(char**)vaArg);
				break;
			case 'x':
				printhex(vt, *(int32*)vaArg);
			}
			vaArg += sizeof(int32);
		}
		else
			vtPutchar(vt, fmtStr[i]);
		i++;
	}
}

static void printhex(uint32 vt, uint32 hex)
{
	printhexw(vt, hex, sizeof(uint32)*2);
}

static void printhexw(uint32 vt, uint32 hex, uint32 count)
{
	char * digits = "0123456789ABCDEF";

	if (count == 0)
		return;

	printhexw(vt, hex/16, count-1);
	vtPutchar(vt, digits[hex % 16]);
}

static void printnum(uint32 vt, uint32 num)
{
	if (num == 0)
	{
		vtPutchar(vt, '0');
		return;
	}
	if (num < 0)
	{
		vtPutchar(vt, '-');
		num *= -1;
	}

	printnat(vt, num);
}

void printnat(uint32 vt, uint32 num)
{
	if (num == 0)
		return;

	printnat(vt, num/10);
	vtPutchar(vt, '0' + num % 10);
}

static void printstr(uint32 vt, char * str)
{
	while (*str != 0)
		vtPutchar(vt, *(str++));
}


//static void printInputBuffer(uint32, char * str)
//{
//
//}
