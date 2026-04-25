#ifndef DA_H
#define DA_H


#include <naiveMemory.h>


typedef struct dynamic_array_char
{
      int32 capacity;
      int32 count;
      char *ptr;
} da_char;

da_char *da_char_init(int32 size);




#endif // DA_H