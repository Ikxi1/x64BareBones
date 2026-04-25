#include <naiveTerminal.h>


void nt_print(char character)
{
      switch (character)
      {
            case 0x08: // BACKSPACE
            { // still needs to be checked for "empty" and after a command was done, it shouldn't delete
                  if (cursor_x > 2)
                  {
                        nc_delete_char();
                  }
                  break;
            }

            case 0x0A: // ENTER
            {
                  // check here for commands

                  da_char *a = da_char_init(10);
                  a->ptr = "hello\0";
                  da_char *b = da_char_init(10);
                  b->ptr = "hello\0";
                  ncPrintBase(strncmp(a, b, 5), 10, 1);

                  // after command ran, new line
                  nc_newline();
                  // nc_print("$>", 0);
                  nc_print(a->ptr, 1);
                  nc_print(b->ptr, 1);
                  break;
            }

            case 0x20: // SPACE
            {
                  nc_print_char(0);
                  break;
            }

            default:
            {
                  nc_print_char(character);
                  break;
            }
      }
}


void naive_terminal()
{
      bool running = true;

      nc_print("$>", 0);

      while (running)
      {
            while (key_event.key == 0)
            {
                  build_key_event();
            }

            nt_print(key_event.key);

            key_event.key = 0;
      }
}


void echo(da_char *string)
{
}