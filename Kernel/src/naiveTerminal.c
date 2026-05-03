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
                  uint32 a = strncmp((char *)get_text_ptr() - 4, "echo", 4);
                  if (a == 0)
                  {
                        nc_print("strings match", 0);
                  }
                  else
                  {
                        nc_print("strings don't match", 0);
                  }
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
      key_event.key = 0;
      nc_clear();

      bool running = true;

      nc_print("$>", 0);

      while (running)
      {
            while (key_event.key == 0)
            {
                  build_key_event();
            }

            if (key_event.key == 0x1B)
            {
                  naive_terminal();
            }

            nt_print(key_event.key);

            key_event.key = 0;
      }
}


void echo(String *string)
{
}