#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <naiveMemory.h>


typedef struct string
{
      uint32 capacity;
      uint32 count;
      char *ptr;
} String;


/* strncmp

 Compare n characters of 2 strings.
 Give back the difference.

 return:
   == 0 -> same string
   != 0 -> different strings
*/
uint32 strncmp_s(String *string1, String *string2, uint32 n);
uint32 strncmp(char *string1, char *string2, uint32 n);


String *new_string(uint32 size);


#endif // STRING_H
