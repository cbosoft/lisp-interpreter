#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenise.h"
#include "gc.h"

#define ADD_TO_TOKENS(VALUE) \
  (*tokens) = realloc( (*tokens), (++(*n_tokens))*sizeof(char*)); \
  (*tokens)[(*n_tokens)-1] = calloc((strlen(VALUE)+1), sizeof(char)); \
  strcpy( (*tokens)[(*n_tokens)-1], VALUE ); // why does strncpy complain here?


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

void tokenise(char *input, char ***tokens, int *n_tokens)
{

  char ch;
  int i = 0;

  char *kw_or_name = NULL;
  int kw_or_name_len = 0;
  int input_len = strlen(input);

  (*tokens) = NULL;
  (*n_tokens) = 0;

  int in_quote = 0;

  for (i = 0, ch= input[0]; i < input_len; i++, ch=input[i]) {

    if (input[i] == ';') {
      for (;input[i] != '\n' && i < input_len; i++);
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

    }
    else {

      if ((ch == '"') || (ch == '\''))  {

        if (((in_quote == 2) && (ch == '"')) || ((in_quote == 1) && (ch == '\''))) {
          in_quote = 0;
        }
        else {
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

}
