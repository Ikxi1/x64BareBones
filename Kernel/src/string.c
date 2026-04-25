#include <string.h>


int32 strncmp(da_char *string1, da_char *string2, int32 n)
{
      if (string1->count != string2->count ||
          string1->capacity != string2->capacity)
      {
            return -1;
      }

      for (int32 i = 0; i < n; i++)
      {
            if (string1->ptr[i] != string2->ptr[i])
            {
                  return -1;
            }
      }
      return 0;
}
