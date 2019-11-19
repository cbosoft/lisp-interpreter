#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenise.h"

#define ADD_TO_ROOT(R, N, V) \
  N++; \
  if (R == NULL) \
    R = calloc(N, sizeof(char*)); \
  else \
    R = realloc(R, N*sizeof(char*)); \
  R[N-1] = calloc((strlen(V)+1), sizeof(char)); \
  strcpy( R[N-1], V ); // why does strncpy complain here?

#define INTERRUPT_KW


void tokenise(char *input, char ***tokens, int *n_tokens)
{

  char **_tokens = NULL;
  int _n_tokens = 0;

  char *kw_or_name = NULL;
  int kw_or_name_len = 0;
  int n_open_parens = 0, line=1, col=1;
  int input_len = strlen(input);

  for (int i = 0; i < input_len; i++) {
    col ++;

    if (input[i] == ';') {
      for (;input[i] != '\n' && i < input_len; i++);
      col = 0;
      line++;
    }
    
    // if breaking char: space, newline, or parens
    if ((input[i] == ' ') || (input[i] == '\n') || (input[i] == ')') || (input[i] == '(')) {

      // finish reading keyword or name
      if (kw_or_name != NULL) {
        kw_or_name[kw_or_name_len] = '\0';
        ADD_TO_ROOT(_tokens, _n_tokens, kw_or_name);
        free(kw_or_name);
        kw_or_name = NULL;
        kw_or_name_len = 0;
      }

      // action needed on breaking char?
      switch (input[i]) {

        case '(':
        // new list
        ADD_TO_ROOT(_tokens, _n_tokens, "(");
        n_open_parens ++;
        break;

      case ')':
        // close list
        if (!n_open_parens) {
          fprintf(stderr, "Unmatched ')'.\n");
        }
        ADD_TO_ROOT(_tokens, _n_tokens, ")");
        n_open_parens --;
        break;

      case '\n':
        col = 1;
        line ++;
        break;

      }

    }
    else {

      kw_or_name_len++;
      kw_or_name = realloc(kw_or_name, (kw_or_name_len + 1)*sizeof(char));
      kw_or_name[kw_or_name_len-1] = input[i];

    }

  }

  (*tokens) = _tokens;
  (*n_tokens) = _n_tokens;

}
