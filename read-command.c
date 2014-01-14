// UCLA CS 111 Lab 1 command reading
#include "command.h"
#include "command-internals.h"

#include <error.h>

#include <stdio.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

enum token_id  {
  semicolon, pipe, and, or, start_subshell, close_subshell, in, out, word, nil
};

struct token_list_element {
  enum token_id id;
  char* body;
};

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
         void *get_next_byte_argument)
{
  printf("%c\n", get_next_byte(get_next_byte_argument));
  error (1, 0, "command reading not yet implemented");
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  error (1, 0, "command reading not yet implemented");
  return 0;
}
