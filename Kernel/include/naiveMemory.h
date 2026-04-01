#ifndef NAIVEMEMORY_H
#define NAIVEMEMORY_H


#include <stdint.h>


extern uintptr *heap_ptr;

void *malloc(uint64 size);

#endif // NAIVEMEMORY_H