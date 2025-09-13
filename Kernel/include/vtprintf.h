#ifndef VTPRINTF_H_
#define VTPRINTF_H_

#include <stdint.h>

void kprintf(char * fmtStrm, ...);
void vtprintf(uint32 vt, char * fmtStr, ...);
void vtprintfp(uint32 vt, char * fmtStr, void * vaArgs);

#endif /*VTPRINTF_H_*/
