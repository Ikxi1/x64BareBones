#include <string.h>


String *new_string(uint32 size)
{
      String *d = (String *)malloc(sizeof(String));
      d->capacity = size;
      d->count = 0;
      d->ptr = (char *)malloc(size);

      return d;
}


uint32 strncmp_s(String *string1, String *string2, uint32 n)
{
      if (string1->count != string2->count ||
          string1->capacity != string2->capacity)
      {
            return 1;
      }

      for (uint32 i = 0; i < n; i++)
      {
            if (string1->ptr[i] != string2->ptr[i])
            {
                  return 1;
            }
      }
      return 0;
}


uint32 strncmp(char *string1, char *string2, uint32 n)
{
      if (string1 == null || string2 == null)
      {
            return 1;
      }

      for (uint32 i = 0; i < n; i++)
      {
            if (string1[i] != string2[i])
            {
                  return 1;
            }
      }

      return 0;
}
