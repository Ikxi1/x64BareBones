#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <da.h>


/* strncmp

Compare n characters of 2 strings.
Give back the difference.
return:
  == 0 -> same string
  != 0 -> different strings
*/
int32 strncmp(da_char *string1, da_char *string2, int32 n);


#endif // STRING_H
