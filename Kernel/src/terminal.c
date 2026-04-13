#include <terminal.h>


void terminal()
{
      bool running = true;

      nc_print("$>", 0);

      while (running)
      {
            while (key_event.key == 0)
            {
                  build_key_event();
            }
            const char *c = (const char*)&key_event.key;

            switch (key_event.key)
            {
                  // case 0x08:
                  // {
                  //       break;
                  // }

                  case 0x0A:
                  {
                        nc_newline();
                        nc_print("$>", 0);
                        break;
                  }

                  default:
                  {
                        nc_print((const char*)&key_event.key, 0);
                        break;
                  }
            }
            key_event.key = 0;
      }
}
