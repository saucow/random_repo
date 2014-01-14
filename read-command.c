// UCLA CS 111 Lab 1 command reading
#include "command.h"
#include "command-internals.h"
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

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

int num_tokens = 0;

void add_token_list_element(struct token_list_element **token_array) {
  num_tokens++;
  printf("%i\n", num_tokens);
  struct token_list_element *tmp = (struct token_list_element *) realloc(token_array, (num_tokens) * sizeof(struct token_list_element));
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
         void *get_next_byte_argument)
{
  int byte_value;

  struct token_list_element *token_array = (struct token_list_element *) malloc(sizeof(struct token_list_element)); 

  while((byte_value = get_next_byte(get_next_byte_argument)) > 0) {
    switch(byte_value) {
      case '|': {
        printf("%c\n", byte_value);
        add_token_list_element(&token_array);
        break;
      }
    }
  }
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
