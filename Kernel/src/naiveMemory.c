#include <naiveMemory.h>


static uint8 endOfKernel; // from linker.ld
uintptr *heap_ptr = 0;


void *align_64(uintptr *ptr)
{
      uintptr p = (uintptr)ptr;
      // this works, calculated it
      return (void *)((p+7) & ~(uintptr)7);
}


void heap_init()
{
      uintptr start = (uintptr)&endOfKernel;
      heap_ptr = (uintptr *)align_64(&start);
}


void *malloc(uint64 size)
{
      if (!heap_ptr) return null; /* not initialized */
      // when not 64 bit aligned, add until it is
      // for example when only 8 bit aligned
      uintptr cur = (uintptr)align_64(heap_ptr);
      uintptr next = cur + size;
      heap_ptr = (uintptr *)next;
      return (void *)cur;
}


void memcpy(void *ptr, void *data, uint64 size)
{
      uint64 c = 0;
      uint8 *p = (uint8 *)ptr;
      uint8 *d = (uint8 *)data;

      // try 8 byte alignment later
      // if (size % 8 != 0)
      // {
      //
      //       while ((size-c) % 8 != 0)
      //       {
      //             c++;
      //       }
      // }

      while (c < size)
      {
            p[c] = d[c];
            c++;
      }
}