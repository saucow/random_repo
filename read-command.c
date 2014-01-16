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

char* create_one_length_string(char a) {
  char* temp = checked_malloc(2 * sizeof(char));
  temp[0] = a;
  temp[1] = '\0';
  return temp;
}

struct token* create_list(int val, char body, token_id type)
{
    printf("\n creating list with headnode as [%d]\n",val);
    struct token *ptr = (struct token*)malloc(sizeof(struct token));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;
    ptr->body = create_one_length_string(body);
    ptr->id = type;

    head = curr = ptr;
    return ptr;
}

struct token* add_to_list(int val, char body, int add_to_end, token_id type)
{
    i++;
    if(NULL == head)
    {
        return (create_list(val, body, type));
    }

    if(add_to_end == 0)
        printf("\n Adding node to end of list with value [%d]\n",val);
    else
        printf("\n Adding node to beginning of list with value [%d]\n",val);

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

    if(add_to_end == 1)
    {
        curr->next = ptr;
        curr = ptr;
    }
    else
    {
        ptr->next = head;
        head = ptr;
    }
          printf("SETTING CURR ID: %i\n", curr->id);
    return ptr;
}

void print_list(void)
{
    struct token *ptr = head;

    printf("\n -------Printing list Start------- \n");
    while(ptr != NULL)
    {
       //printf("\n [%d] \n",ptr->id);
       //printf("\n [%d] \n",ptr->val);
        if(ptr->body != NULL) {
          printf("%s\n", ptr->body);
        }
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");

    return;
}

char* str_cat_char(char* string, char new_char) //helper function to append a char to a str
{

    int length = strlen(string)+1;
    string = checked_realloc(string, length);
    string[length-1] = new_char;
    string[length] = 0;
    return string;
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
        else { //Only 1  &???

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
