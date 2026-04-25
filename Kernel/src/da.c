#include <da.h>


da_char *da_char_init(int32 size)
{
      da_char *d = (da_char *)malloc(sizeof(da_char));
      d->capacity = size;
      d->count = 0;
      d->ptr = (char *)malloc(size);

      return d;
}
