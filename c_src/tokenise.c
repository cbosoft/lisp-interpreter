#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenise.h"
#include "exception.h"
#include "debug.h"
#include "gc.h"

#define IS_WHITESPACE(CH) ((CH == ' ') || (CH == '\n') || (CH == '\t'))

#define ADD_TO_TOKENS(VALUE) \
  new = malloc(sizeof(LispToken)); \
  new->string = calloc((strlen(VALUE)+1), sizeof(char)); \
  strcpy( new->string, VALUE );\
  new->line = line_no;\
  new->col = col_no;\
  new->next = NULL;\
  new->file = source_cpy;\
  if (current != NULL) {\
    current->next = new;\
  }\
  else {\
    rv = new;\
  }\
  current = new;


// simple sanity check over whether all parentheses are closed
int parenscheck(char *input)
{
  char ch;
  int len = strlen(input), i, nopen = 0, nclosed = 0;

  
  for (i = 0, ch = input[0]; i < len; i++, ch = input[i]) {

    // TODO: what if the parenthesis is in a string?
    if (ch == '(') {
      nopen++;
    }
    else if (ch == ')') {
      nclosed++;
    }

  }

  return nopen - nclosed;
}




// Convert input into list of tokens
LispToken *tokenise(char *input, char *source)
{

  char ch, nch;
  int i = 0;

  char *kw_or_name = NULL;
  int kw_or_name_len = 0;
  int input_len = strlen(input);
  int in_quote = 0;
  int line_no = 1;
  int col_no = 1;
  int parens_level = 0;
  int add_close_parens_on_break = 0;
  int add_close_parens_on_parens = 0;

  char *source_cpy = strdup(source);

  LispToken *rv = NULL, *current = rv, *new = NULL;

  for (i = 0, ch = input[0], nch=input[1]; i < input_len; ch = input[++i], nch=input[i+1]) {
    col_no ++;

    if (input[i] == ';') {
      for (;input[i] != '\n' && i < input_len; i++, line_no++);
      col_no = 1;
      continue;
    }
    
    // if breaking char: space, newline, or parens
    if (( IS_WHITESPACE(ch) || (ch == ')') || (ch == '(') || (ch == '\'')) && !in_quote) {

      // finish reading keyword or name
      if (kw_or_name != NULL) {
        kw_or_name[kw_or_name_len] = '\0';
        ADD_TO_TOKENS(kw_or_name);
        kw_or_name = NULL;
        kw_or_name_len = 0;

        if (add_close_parens_on_break) {
          add_close_parens_on_break = 0;
          ADD_TO_TOKENS(")");
        }
      }

      // TODO switch-case
      // action needed on breaking char?
      if (ch == '(') {
        ADD_TO_TOKENS("(");
        parens_level++;
      }
      if (ch == ')') {
        ADD_TO_TOKENS(")");
        parens_level--;

        if (add_close_parens_on_parens) {
          add_close_parens_on_parens = 0;
          ADD_TO_TOKENS(")");
        }
      }
      else if (ch == '\n') {
        col_no = 1;
        line_no ++;
      }
      else if (ch == '\'') {

        ADD_TO_TOKENS("(");
        ADD_TO_TOKENS("quote");

        if (nch == '('){
          debug_message("NEXT CHAR IS '('; quote list\n");
          add_close_parens_on_parens = 1;
        }
        else if (IS_WHITESPACE(nch)) {
          //error
          debug_message("NEXT CHAR IS WHITE SPACE! ERROR");
          Exception_raise("SyntaxError", "tokenise", NULL, "single quote should be before a list or other object.");
        }
        else {
          add_close_parens_on_break = 1;
          debug_message("NEXT CHAR IS '('; quote kw\n");
        }
      }

    }
    else {

      if (ch == '"')
        in_quote = !in_quote;

      kw_or_name = realloc(kw_or_name, ((++kw_or_name_len) + 1)*sizeof(char));
      kw_or_name[kw_or_name_len-1] = ch;

    }

  }

  if (kw_or_name != NULL) {
    kw_or_name[kw_or_name_len] = '\0';
    ADD_TO_TOKENS(kw_or_name);
    kw_or_name = NULL;
    kw_or_name_len = 0;

    if (add_close_parens_on_break) {
      add_close_parens_on_break = 0;
      ADD_TO_TOKENS(")");
    }
  }

  return rv;
}
