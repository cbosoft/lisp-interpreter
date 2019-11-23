#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>

#include "parse.h"
#include "ast.h"
#include "object.h"
#include "exception.h"
#include "debug.h"
#include "list.h"


pcre *int_regexp = NULL;
pcre *float_regexp = NULL;
pcre *string_regexp = NULL;
const char *pcre_error = NULL;
int pcre_error_offset = 0;



void parser_init()
{
  int_regexp = pcre_compile("^-?\\d+$", 0, &pcre_error, &pcre_error_offset, NULL);
  assert_or_error(int_regexp != NULL, "parser_init", "Regex (int) compilation failed: %s", pcre_error);
  if (Exception_check()) return;

  float_regexp = pcre_compile("^-?\\d+(\\.|e\\+|e-|e)\\d+$", 0, &pcre_error, &pcre_error_offset, NULL);
  assert_or_error(float_regexp != NULL, "parser_init", "Regex (float) compilation failed: %s", pcre_error);
  if (Exception_check()) return;

  string_regexp = pcre_compile("^\".*\"$|^'.*'$", 0, &pcre_error, &pcre_error_offset, NULL);
  assert_or_error(float_regexp != NULL, "parser_init", "Regex (string) compilation failed: %s", pcre_error);
}




// Create new object, guessing type
LispObject *new_object_guess_type(char *s) {
  int len = strlen(s);

  debug_message("GUESSING %s\n", s);
  
  if (strcmp(s, "(") == 0) {
    debug_message("GUESSING %s is LIST\n", s);
    return LispObject_new_list();
  }

  if (strcmp(s, "false") == 0) {
    debug_message("GUESSING %s is BOOL\n", s);
    return LispObject_new_bool(0);
  }

  if (strcmp(s, "true") == 0) {
    debug_message("GUESSING %s is BOOL\n", s);
    return LispObject_new_bool(1);
  }

  // PCRE FTW
  int ovector[30];
  int rc = pcre_exec( int_regexp, NULL, s, len, 0, 0, ovector, 30);

  if (rc >= 0) {
    debug_message("GUESSING %s is INT\n", s);
    return LispObject_new_int(atoi(s));
  }

  rc = pcre_exec( float_regexp, NULL, s, len, 0, 0, ovector, 30);

  if (rc >= 0) {
    debug_message("GUESSING %s is FLOAT\n", s);
    return LispObject_new_float(atof(s));
  }

  rc = pcre_exec( string_regexp, NULL, s, len, 0, 0, ovector, 30);

  if (rc >= 0) {
    debug_message("GUESSING %s is STRING\n", s);
    return LispObject_new_string(s);
  }

  return LispObject_new_symbol(s);
}



LispListElement *parse(char **tokens, int n_tokens)
{
  // TODO: root should, instead of a list object, be just a linked list (no object).
  LispListElement *root = LispList_new_element();
  LispObject *current_list = NULL, *new = NULL, **open_lists= NULL;

  debug_message("IN PARSE\n");

  int n_open_lists = 0;

  for (int i = 0; i < n_tokens; i++) {
    debug_message("TOKEN: %s\n", tokens[i]);

    if (strcmp(tokens[i], "(") == 0) {
      debug_message("OPENING NEW LIST\n");
      new = LispObject_new_list();
      ERROR_CHECK;
      debug_message("after OPENING NEW LIST\n");

      if (current_list != NULL) {
        debug_message("ADD NEW LIST TO PARENT\n");
        open_lists = realloc(open_lists, (++n_open_lists)*sizeof(LispObject *));
        open_lists[n_open_lists-1] = current_list;
        LispList_add_object_to_list(current_list->value_list, new);
        ERROR_CHECK;
      }
      else {
        debug_message("ADD NEW LIST TO ROOT\n");
        LispList_add_object_to_list(root, new);
        ERROR_CHECK;
      }

      current_list = new;
    }
    else if (strcmp(tokens[i], ")") == 0) {
      debug_message("CLOSING LIST\n");

      if (n_open_lists) {
        current_list = open_lists[(--n_open_lists)];
        open_lists = realloc(open_lists, n_open_lists*sizeof(LispObject *));
      }
      else {
        current_list = NULL;
        open_lists = NULL;
      }

    }
    else {
      debug_message("GUESS\n");
      new = new_object_guess_type(tokens[i]);
      ERROR_CHECK;

      debug_message("NEW OBJECT %s (%s)\n", LispObject_repr(new), LispObject_type(new));

      if (current_list != NULL) {
        debug_message("ADD TO LIST\n");
        LispList_add_object_to_list(current_list->value_list, new);
        ERROR_CHECK;
      }
      else {
        debug_message("ADD TO ROOT\n");
        LispList_add_object_to_list(root, new);
        ERROR_CHECK;
      }
    }

  }

  return root;
}
