#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenise.h"
#include "gc.h"

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

  char ch;
  int i = 0;

  char *kw_or_name = NULL;
  int kw_or_name_len = 0;
  int input_len = strlen(input);
  int in_quote = 0;
  int line_no = 0;
  int col_no = 0;

  char *source_cpy = strdup(source);

  LispToken *rv = NULL, *current = rv, *new = NULL;


  for (i = 0, ch = input[0]; i < input_len; i++, ch=input[i]) {
    col_no ++;

    if (input[i] == ';') {
      for (;input[i] != '\n' && i < input_len; i++, line_no++);
      col_no = 0;
      continue;
    }
    
    // if breaking char: space, newline, or parens
    if (((ch == ' ') || (ch == '\n') || (ch == ')') || (ch == '(') || (ch == '\t')) && !in_quote) {

      // finish reading keyword or name
      if (kw_or_name != NULL) {
        kw_or_name[kw_or_name_len] = '\0';
        ADD_TO_TOKENS(kw_or_name);
        kw_or_name = NULL;
        kw_or_name_len = 0;
      }

      // action needed on breaking char?
      if ((ch == '(') || (ch == ')')) {
        ADD_TO_TOKENS(ch == '(' ? "(" : ")");
      }
      else if (ch == '\n') {
        col_no = 0;
        line_no ++;
      }

    }
    else {

      if ((ch == '"') || (ch == '\''))  {

        if (((in_quote == 2) && (ch == '"')) || ((in_quote == 1) && (ch == '\''))) {
          in_quote = 0;
        }
        else if (!in_quote) {
          in_quote = ch == '"' ? 2 : 1;
        }

      }

      kw_or_name = realloc(kw_or_name, ((++kw_or_name_len) + 1)*sizeof(char));
      kw_or_name[kw_or_name_len-1] = ch;

    }

  }

  if (kw_or_name != NULL) {
    kw_or_name[kw_or_name_len] = '\0';
    ADD_TO_TOKENS(kw_or_name);
    kw_or_name = NULL;
    kw_or_name_len = 0;
  }

  return rv;
}
