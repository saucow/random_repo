// UCLA CS 111 Lab 1 command reading
#include "command.h"
#include "command-internals.h"
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

typedef enum {
  semicolon, pipe, and, or, start_subshell, close_subshell, in, out, word, nil
}token_id;

struct token
{
    int val;
    token_id id;
    char* body;
    struct token *next;
};

int i = 0;

struct token *head = NULL;
struct token *curr = NULL;
struct token *scan_head = NULL;

char* create_one_length_string(char a) {
  char* temp = checked_malloc(2 * sizeof(char));
  temp[0] = a;
  temp[1] = '\0';
  return temp;
}

struct token* create_list(int val, char body, token_id type) {
    struct token *ptr = (struct token*)malloc(sizeof(struct token));
    ptr->val = val;
    ptr->next = NULL;
    ptr->body = create_one_length_string(body);
    ptr->id = type;

    head = curr = ptr;
    return ptr;
}

struct token* add_to_list(int val, char body, int add_to_end, token_id type) {
    i++;
    if(NULL == head) {
      return (create_list(val, body, type));
    }

    /*
    if(add_to_end == 0)
        printf("\n Adding node to end of list with value [%d]\n",val);
    else
        printf("\n Adding node to beginning of list with value [%d]\n",val);
    */

    struct token *ptr = (struct token*)malloc(sizeof(struct token));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->body = create_one_length_string(body);
    ptr->next = NULL;
    ptr->id = type;

    if(add_to_end == 1) {
        curr->next = ptr;
        curr = ptr;
    }
    else {
        ptr->next = head;
        head = ptr;
    }
    return ptr;
}

void print_list(void) {
    struct token *ptr = head;

    printf("\n -------Print list------- \n");
    while(ptr != NULL) {
       //printf("\n [%d] \n",ptr->id);
       //printf("\n [%d] \n",ptr->val);
        if(ptr->body != NULL) {
          printf("%s\n", ptr->body);
        }
        ptr = ptr->next;
    }
    printf("\n -------------- \n");

    return;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
         void *get_next_byte_argument)
{
  int byte_value;
  int next_byte_value = 0;
  struct token *ptr = NULL;

  byte_value = get_next_byte(get_next_byte_argument);

  while(byte_value > 0) {

    switch(byte_value) {
      case ';': {
        add_to_list(i,byte_value,1,semicolon);
        break;
      }
      case '<': {
        add_to_list(i,byte_value,1,in);
        break;
      }
      case '>': {
        add_to_list(i,byte_value,1,out);
        break;
      }
      case '(': {
        add_to_list(i,byte_value,1,start_subshell);
        break;
      }
      case ')': {
        add_to_list(i,byte_value,1,close_subshell);
        break;
      }
      case '|': {
        if(curr != NULL) { 
          if(curr->id == pipe) { //Found ||
            curr->id = or; 
            char* temp = checked_malloc(3 * sizeof(char));
            temp[0] = '|';
            temp[1] = '|';
            temp[2] = '\0';
            curr->body = temp; 
          }
          else { //Add |
            add_to_list(i,byte_value,1,pipe);
          }
        }
        else { //Add |
            add_to_list(i,byte_value,1,pipe);
        }
        break;
      }
      case '&': { //&&
        int temp_byte_value = get_next_byte(get_next_byte_argument);
        if(temp_byte_value == '&') {
          add_to_list(i,byte_value,1,and);
          int str_length = strlen(curr->body)+1;
          char* new_string = checked_realloc(curr->body, str_length);
          new_string[str_length-1] = byte_value;
          new_string[str_length] = '\0';
          curr->body = new_string;
        }
        else { //Only 1 throw error

        }
        break;
      }
      case '#': { //Comment
        int temp_byte_value = get_next_byte(get_next_byte_argument);
        while(temp_byte_value != '\n') {
          temp_byte_value = get_next_byte(get_next_byte_argument);
        }
        break;
      }
      case ' ': { //Spaces
        if(curr->id != word) { 
          break;
        }
      }
      default://Word
      {
        if(curr != NULL) { 
            if(curr->id == word) { 
              int str_length = strlen(curr->body)+1;
              char* new_string = checked_realloc(curr->body, str_length);
              new_string[str_length-1] = byte_value;
              new_string[str_length] = '\0';
              curr->body = new_string;
            }
            else { 
              add_to_list(i,byte_value,1,word);
            }
          }
          else { 
              add_to_list(i,byte_value,1,word);
          }
        }
    }
    byte_value = get_next_byte(get_next_byte_argument);
  }
  print_list();
  return 0;
}


command_t look_for_simple(struct token *ptr) { //Fourth highest scan precedence
  if(ptr->id == word) {
    command_t simple_command = malloc(sizeof(struct command));
    simple_command->type = SIMPLE_COMMAND;
    simple_command->input = 0;
    simple_command->output = 0;
    simple_command->u.word = &ptr->body;

    ptr = ptr->next;
    //Input
    if(ptr->id == in) {
      printf("\nfound <");
      ptr = ptr->next;
      if(ptr->id == word) {
        simple_command->input = ptr->body;
        printf("\ninput: %s\n", ptr->body);
        return simple_command;
      }
      else {
        printf("\nnot word throw error");
      }
    }
    print_list();
    //Output
    if(ptr->id == out) {
      printf("\nfound >");
      ptr = ptr->next;
      if(ptr->id == word) {
        simple_command->output = ptr->body;
        printf("\noutput: %s\n", ptr->body);
        printf("\ngot output");
        return simple_command;
      }
      else {
        printf("\nnot word throw error");
      }
    }
  }
  return NULL;
}

command_t look_for_subshell() { //Third highest scan precedence
  struct token *ptr = scan_head;
  if(ptr->id == start_subshell) {

  }
  return NULL;
}

command_t look_for_pipe(struct token *ptr) { //Second highest scan precedence
  //Scan lower precedence 
  command_t command_result = look_for_subshell();
  if(command_result == NULL) {
    command_result = look_for_simple(ptr);
  }
  printf("\ndone looking for first part of pipe");
  //Look for Pipe
  ptr = ptr->next;
  while(ptr != NULL) {
    if(ptr->id == pipe) {
      command_t second_result = look_for_subshell();
      if(second_result == NULL) {
        second_result = look_for_simple(ptr);
      }
      command_t pipe_command = malloc(sizeof(struct command));
      pipe_command->type = PIPE_COMMAND;
      pipe_command->input = 0;
      pipe_command->output = 0;
      pipe_command->u.command[1]= second_result;
      pipe_command->u.command[0]= command_result;
      return pipe_command;
    }
    ptr = ptr->next;
  }
  return command_result;
}

command_t read_command_stream (command_stream_t s) {
    scan_head = head;
    if(scan_head != NULL) {

      command_t command_result = look_for_pipe(scan_head);
      scan_head = scan_head->next;

      while(scan_head != NULL) {
        struct token *inner_while_ptr = scan_head;
        if(scan_head->id == or || scan_head->id == and) { //Scan highest precedence

          struct token *ptr = scan_head;
          ptr = ptr->next;
          if(ptr == NULL) {
            break; 
          }

          command_t second_result = look_for_pipe(ptr);
          printf("\ndone looking for second part of pipe");

         command_t command_and_or  = malloc(sizeof(struct command));
         command_and_or->type = OR_COMMAND;
         if(ptr->id == and) {
           command_and_or->type = AND_COMMAND;
         }
         command_and_or->u.command[0] = command_result;
         command_and_or->u.command[1] = second_result;
         return command_and_or;
          
        } 
        scan_head = scan_head->next;
      }
    }
  //Exit with error
  return 0;
}
