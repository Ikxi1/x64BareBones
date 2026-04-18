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
                  

                  // after command ran, new line
                  nc_newline();
                  nc_print("$>", 0);
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
