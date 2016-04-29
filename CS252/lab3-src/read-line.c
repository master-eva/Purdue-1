/*
 * CS354: Operating Systems.
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "read-line.h"
#include "command.h"

#define MAX_BUFFER_LINE 2048

// Buffer where line is stored
int line_length;
int line_position;
char line_buffer[MAX_BUFFER_LINE];

// Simple history array
// This history does not change.
// Yours have to be updated.
int history_max = 2;
int history_length = 0;
int history_index = 0;
char ** history = NULL;

void read_line_print_usage()
{
  const char * usage = "\n\r"
                       " ctrl-?       Print usage\n\r"
                       " ctrl-A       Move to beginning of the line\n\r"
                       " ctrl-E       Move to end of the line\n\r"
                       " Backspace    Deletes last character\n\r"
                       " up arrow     See last command in the history\n\r"
                       " down arrow   See next command in the history\n\r";

  write(1, usage, strlen(usage));
  Command::_currentCommand.clear();
  Command::_currentCommand.prompt();
}

/*
 * Input a line with some basic editing.
 */
char * read_line() {
  struct termios orig_attr;
  tcgetattr(0, &orig_attr);

  // Set terminal in raw mode
  tty_raw_mode();

  line_length = 0;
  line_position = 0;

  // init history
  if (history == NULL) {
    history = (char **) malloc (history_max * sizeof(char*));
  }

  // Read one line until enter is typed
  while (1) {
    // Read one character in raw mode.
    char ch;
    read(0, &ch, 1);

    if (ch >= 32 && ch < 127) {
      // It is a printable character.
      if (line_position == line_length) {
        write(1, &ch, 1);

        // If max number of character reached return.
        if (line_length == MAX_BUFFER_LINE - 2) break;

        // add char to buffer.
        line_buffer[line_length] = ch;
        line_length++;
        line_position++;
      } else {
        line_length++;
        if (line_length == MAX_BUFFER_LINE - 2) break;

        int i;
        //move all char after pos 1 position
        for ( i = line_length; i > line_position; i--)
          line_buffer[i] = line_buffer[i - 1];
        line_buffer[line_position] = ch;

        // write the char
        write(1, &line_buffer[line_position++], 1);

        // write the following chars
        for (i = line_position; i < line_length; i++)
          write(1, &line_buffer[i], 1);

        //backspace to the pos
        for (i = line_position; i < line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }
      }
    } else if (ch == 10 || ch == 13) {
      // <Enter> was typed. Return line
      if (strlen(line_buffer) != 0) {
        // resizing history
        if (history_length == history_max - 1) {
          history_max *= 2;
          history = (char**) realloc (history, history_max * sizeof(char*));
        }

        // add line
        char * new_history = strdup(line_buffer);
        new_history [ line_length ] = '\0';
        history[ history_length++ ] = new_history;
        history_index++;
      }

      history_index = history_length;
      // Print newline
      ch = 10;
      write(1, &ch, 1);
      ch = 13;
      write(1, &ch, 1);

      break;
    } else if (ch == 31) {
      // ctrl-?
      read_line_print_usage();
      line_buffer[0] = 0;
      break;
    } else if (ch == 8 || ch == 127) {
      // <backspace> was typed. Remove previous character read.
      if (line_position <= 0) {
        continue;
      } else if (line_position == line_length) {
        // Go back one character
        ch = 8;
        write(1, &ch, 1);

        // Write a space to erase the last character read
        ch = ' ';
        write(1, &ch, 1);

        // Go back one character
        ch = 8;
        write(1, &ch, 1);

        line_buffer[ line_length ] = '\0';
        line_length--;
        line_position--;

      } else {
        int i;

        for (i = line_position - 1; i < line_length - 1; i++)
          line_buffer[i] = line_buffer[i + 1];
        line_buffer[line_length - 1] = '\0';

        for (i = 0; i < line_position; i++) {
          ch = 8;
          write(1, &ch, 1);
        }
        for (i = 0; i < line_length; i++) {
          ch = ' ';
          write(1, &ch, 1);
        }

        for (i = 0; i < line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        //Write in the line buffer
        for (i = 0; i < line_length - 1; i++)
          write(1, &line_buffer[i], 1);

        // Move back to the position
        for (i = 0; i < (line_length - line_position); i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        // Remove one character
        line_position--;
        line_length--;
      }
    }else if (ch == 4) {
      //delete key(ctrl-D): Removes the character at the cursor.
      if (line_position < 0) continue;
      if (line_position == line_length) continue;
      int i;

      for (i = line_position; i < line_length - 1; i++)
        line_buffer[i] = line_buffer[i + 1];
      line_buffer[line_length - 1] = '\0';

      for (i = 0; i < line_position; i++) {
        ch = 8;
        write(1, &ch, 1);
      }
      for (i = 0; i < line_length; i++) {
        ch = ' ';
        write(1, &ch, 1);
      }

      for (i = 0; i < line_length; i++) {
        ch = 8;
        write(1, &ch, 1);
      }

      //Write in the line buffer
      for (i = 0; i < line_length - 1; i++)
        write(1, &line_buffer[i], 1);

      //Move back to the position
      for (i = 0; i < (line_length - line_position - 1); i++) {
        ch = 8;
        write(1, &ch, 1);
      }

      // Remove one character
      line_length--;
    } else if (ch == 1) {
      // Home key (or ctrl-A): The cursor moves to the beginning of the line;
      int i;
      for (i = 0; i < line_position; i++) {
        ch = 8;
        write(1, &ch, 1);
      }
      line_position = 0;
    } else if (ch == 5) {
      //End key (or ctrl-E): The cursor moves to the end of the line
      int i;
      for (i = line_position; i < line_length; i++) {
        char ch = line_buffer[i];
        write(1, &ch, 1);
      }
      line_position = line_length;
    } else if (ch == 3) {
      ch = 10;
      write(1, &ch, 1);
      ch = 13;
      write(1, &ch, 1);

      Command::_currentCommand.clear();
      Command::_currentCommand.prompt();

      int i;
      // reset buffer
      for (i = 0; i < line_length; i++)
        line_buffer[i] = '\0';

      line_position = 0;
      line_length = 0;
    } else if (ch == 27) {
      // Escape sequence. Read two chars more
      //
      // HINT: Use the program "keyboard-example" to
      // see the ascii code for the different chars typed.
      //
      char ch1;
      char ch2;
      read(0, &ch1, 1);
      read(0, &ch2, 1);

      if (ch1 == 91 && ch2 == 65) {
        // Up arrow. Print next line in history.
        if (history_length < 1) continue;
        if (history_index  < 1) continue;

        // Erase old line
        // Print backspaces
        int i = 0;
        for (i = 0; i < line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        // Print spaces on top
        for (i = 0; i < line_length; i++) {
          ch = ' ';
          write(1, &ch, 1);
        }

        // Print backspaces
        for (i = 0; i < line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        history_index = history_index - 1;
        strcpy(line_buffer, history[history_index]);
        line_length = strlen(line_buffer);
        line_position = line_length;

        write(1, line_buffer, line_length);
      } else if (ch1 == 91 && ch2 == 66) {
        // down arrow
        if (history_length < 1) continue;
        if (history_index >= history_length - 1) continue;

        int i = 0;
        for (i = 0; i < line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        // Print spaces on top
        for (i = 0; i < line_length; i++) {
          ch = ' ';
          write(1, &ch, 1);
        }

        // Print backspaces
        for (i = 0; i < line_length; i++) {
          ch = 8;
          write(1, &ch, 1);
        }

        history_index = history_index + 1;
        strcpy(line_buffer, history[history_index]);
        line_length = strlen(line_buffer);
        line_position = line_length;

        write(1, line_buffer, line_length);

      } else if (ch1 == 91 && ch2 == 68) {
        // left arrow
        if (line_position > 0) {
          line_position--;
          ch = 8;
          write(1, &ch, 1);
        }
      } else if (ch1 == 91 && ch2 == 67) {
        // right arrow
        if (line_position < line_length) {
          char c = line_buffer[line_position];
          write( 1, &c, 1);
          line_position++;
        }
      } else if (ch1 == 79 && ch2 == 72) {
        //home key
        // Home key (or ctrl-A): The cursor moves to the beginning of the line;
        int i;
        for (i = 0; i < line_position; i++) {
          ch = 8;
          write(1, &ch, 1);
        }
        line_position = 0;

      } else if (ch1 == 79 && ch2 == 70) {
        //endkey
        //End key (or ctrl-E): The cursor moves to the end of the line
        int i;
        for (i = line_position; i < line_length; i++) {
          char ch = line_buffer[i];
          write(1, &ch, 1);
        }
        line_position = line_length;
      }

    }
  }

  // Add eol and null char at the end of string
  line_buffer[line_length] = 10;
  line_length++;
  line_buffer[line_length] = 0;

  tcsetattr(0, TCSANOW, &orig_attr);
  return line_buffer;
}

