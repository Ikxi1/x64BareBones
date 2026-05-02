#ifndef NAIVEMEMORY_H
#define NAIVEMEMORY_H


#include <stdint.h>


extern uintptr *heap_ptr;

void heap_init();

/* malloc

 This allocates memory and gives back a pointer to that allocated
 memory. 8 bytes in front of the pointer, a check value and
 the size of the allocated memory is written as an unsigned
 64 bit integer.

 The check value is 0xDEADBEEF, if it isn't 0xDEADBEEF, a buffer
 overflow occured.

 Parameter:
      Inputs:
      - uint64 size: how much memory you want to allocate

      Outputs:
      - void* : a pointer to that memory
*/
void *malloc(uint32 size);

void memcpy(void *ptr, const void *data, uint64 size);


#endif // NAIVEMEMORY_H