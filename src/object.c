#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "debug.h"
#include "types.h"




// Create new string object.
LispObject *LispObject_new_string(char *value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));

  rv->type = LISPOBJECT_STRING;

  rv->value_string = calloc(strlen(value)+1, sizeof(char));
  strcpy(rv->value_string, value);

  return rv;
}




// Create new integer.
LispObject *LispObject_new_int(int value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_INT;
  rv->value_int = value;
  return rv;
}




// Create new float.
LispObject *LispObject_new_float(double value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_FLOAT;
  rv->value_float = value;
  return rv;
}




// Create new boolean.
LispObject *LispObject_new_bool(int value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_BOOL;
  rv->value_bool = value;
  return rv;
}




// Create new symbol.
LispObject *LispObject_new_symbol(char *name)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_SYMBOL;
  rv->symbol_name = strdup(name);
  return rv;
}




// Create new list object.
// A blank list can have objects added to it using LispObject_add_object_to_list(LispObject *obj).
LispObject *LispObject_new_list()
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_LIST;
  return rv;
}




// Create a new LispObject, inferring type from string $s.
// if it contains only digits 0 to 9, its an integer. If it also contains 'e',
// '.', '+' or '-' then its a float. If its a left parens '(' then its a list.
// If its 'false' or 'true', then its a bool. Otherwise, its a string.
LispObject *LispObject_new_guess_type(char *s) {
  int len = strlen(s), i, ch;

  debug_message("GUESSING %s\n", s);
  
  int not_integer = 0, type = -1;

  if (strstr(s, "\"") != NULL || strstr(s, "'") != NULL) {
    debug_message("GUESSING %s is STRING\n", s);
    return LispObject_new_string(s);
  }

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

  debug_message("GUESSING %s is PROBABLY NUMBER OR SYMBOL\n", s);
  for (i = 0, ch=(int)s[0]; i < len; i++, ch=(int)s[i]) {

    // if char is not number 0-9...
    if ((ch < (int)'0') || (ch > (int)'9')) {

      // if is period '.', then is probably float
      if (ch == (int)'.'){
        not_integer = 1;
      }
      else if (ch == (int)'e') {

        if (len > 1)
          not_integer = 1;
        else {

          type = LISPOBJECT_SYMBOL;
          goto end;

        }

      }
      else if ((ch == (int)'+') || (ch == (int)'-') ) {

        not_integer = 1;
        if (len == 1) {

          type = LISPOBJECT_SYMBOL;
          goto end;

        }

      }
      else {

        type = LISPOBJECT_SYMBOL;
        goto end;

      }

    }

  }

  if (not_integer)
    type = LISPOBJECT_FLOAT;
  else
    type = LISPOBJECT_INT;

end:

  assert_or_error(type >= 0, "LispObject_new_guess_type", "could not guess object type for %s", s);
  ERROR_CHECK;

  switch (type) {

    case LISPOBJECT_INT:
      debug_message("Guessing %s is Int\n", s);
      return LispObject_new_int(atoi(s));

    case LISPOBJECT_FLOAT:
      debug_message("Guessing %s is Float\n", s);
      return LispObject_new_float(atof(s));

    case LISPOBJECT_BOOL:
      debug_message("Guessing %s is Bool\n", s);
      return LispObject_new_bool(strcmp(s, "true") == 0);

    case LISPOBJECT_STRING:
      debug_message("Guessing %s is String\n", s);
      return LispObject_new_symbol(s);

    case LISPOBJECT_SYMBOL:
      debug_message("Guessing %s is SYMBOL\n", s);
      return LispObject_new_symbol(s);
  }

  return NULL;
}




// Add object $toadd to list $list.
// $list must be a list.
void LispObject_add_object_to_list(LispObject *list, LispObject *toadd)
{
  assert_or_error(list->type == LISPOBJECT_LIST, "add_object_to_list", "Cannot add to non-list object.");
  if (Exception_check()) return;

  if (list->list_child == NULL)
    list->list_child = toadd;
  else {

    LispObject *i = list->list_child;

    while (i->list_next != NULL) i = i->list_next;
    // now i has the last item in the linked list
    
    i->list_next = toadd;

  }
}




// Print the value of $o, recursively Note: does not print a newline at end, so
// user should call "LispObject_print" (no trailing underscore) which calls this
// function, and adds the required newline.
void LispObject_print_(LispObject *o) { 

  LispObject *i = NULL; 
  FILE *fp = stdout;

  switch (o->type) {

    case LISPOBJECT_STRING:
      fprintf(fp, "%s", o->value_string);
      break;

    case LISPOBJECT_SYMBOL:
      fprintf(fp, "%s", o->symbol_name);
      break;

    case LISPOBJECT_INT:
      fprintf(fp, "%d", o->value_int);
      break;

    case LISPOBJECT_FLOAT:
      fprintf(fp, "%f", o->value_float);
      break;

    case LISPOBJECT_BOOL:
      fprintf(fp, "%s", (o->value_bool)?"true":"false");
      break;

    case LISPOBJECT_LIST:
      
      i = o->list_child;
      fprintf(fp, " ( ");

      while (i != NULL) {
        LispObject_print_(i);
        i = i->list_next;

        if (i != NULL)
          fprintf(fp, " ");
      }

      fprintf(stdout, " ) ");
      break;

  }
}

void LispObject_print(LispObject *o)
{
  LispObject_print_(o);
  fprintf(stdout, "\n");
}




// Recursively free object $o
void LispObject_free(LispObject *o)
{
  if (o->list_next)
    LispObject_free(o->list_next);

  if (o->list_child)
    LispObject_free(o->list_child);

  free(o);
}



void LispObject_make_nil(LispObject *o)
{
  if (o->value_string != NULL) {
    free(o->value_string);
    o->value_string = NULL;
  }

  o->value_int = 0;
  o->value_float = 0.0;
  o->value_bool = 0;
  o->list_child = NULL;

}




// Copy value from 4source into $dest
void LispObject_assign_value(LispObject *dest, LispObject *source)
{
  LispObject_make_nil(dest);

  switch(source->type) {
    case LISPOBJECT_STRING:
      dest->value_string = calloc(strlen(source->value_string)+1, sizeof(char));
      strcpy(dest->value_string, source->value_string);
      break;

    case LISPOBJECT_INT:
      dest->value_int = source->value_int;
      break;

    case LISPOBJECT_FLOAT:
      dest->value_float = source->value_float;
      break;

    case LISPOBJECT_BOOL:
      dest->value_bool = source->value_bool;
      break;

    case LISPOBJECT_LIST:
      dest->list_child = source->list_child;
      break;
  }

  dest->type = source->type;
}




// Create a copy of an object $o, including children and next items in lists.
LispObject *LispObject_deepcopy(LispObject *o)
{
  LispObject *rv = malloc(sizeof(LispObject));

  assert_or_error(rv != NULL, "LispObject_deepcopy", "memory allocation failed; are you out of memory?");

  LispObject_assign_value(rv, o);

  if (o->list_child != NULL) {
    rv->list_child = LispObject_deepcopy(o->list_child);
  }
  
  if (o->list_next != NULL) {
    rv->list_next = LispObject_deepcopy(o->list_next);
  }

  return rv;
}




// Create a copy of the object $o, not including references to next but
// including references to children. i.e. doesn't return the cdr of the list,
// but if the object IS a list, it will be copied. This is a recursive
// operation, be careful copying lists!
LispObject *LispObject_copy(LispObject *o)
{
  LispObject *rv = malloc(sizeof(LispObject));

  assert_or_error(rv != NULL, "LispObject_copy", "memory allocation failed; are you out of memory?");

  LispObject_assign_value(rv, o);

  if (o->list_child != NULL) {
    rv->list_child = LispObject_deepcopy(o->list_child);
  }

  return rv;
}




// Get string representation of type of object $o
char *LispObject_type(LispObject *o)
{
  switch (o->type)
  {
    case LISPOBJECT_SYMBOL:
      return "Symbol";
    case LISPOBJECT_INT:
      return "Integer";
    case LISPOBJECT_FLOAT:
      return "Float";
    case LISPOBJECT_BOOL:
      return "Boolean";
    case LISPOBJECT_STRING:
      return "String";
  }

  return "List";
}




// Recursively get length of list
int LispObject_list_size(LispObject *o)
{
  return o->list_next ? 1 + LispObject_list_size(o->list_next) : 1;
}




// Represent object as a string
char *LispObject_repr(LispObject *o)
{
  char *rv;
  if (o->type == LISPOBJECT_STRING) {
    rv = calloc(strlen(o->value_string)+3, sizeof(char));
    sprintf(rv, "'%s'", o->value_string);
  }
  if (o->type == LISPOBJECT_SYMBOL) {
    rv = strdup(o->symbol_name);
  }
  else {
  
  rv = calloc(100, sizeof(char));

    switch (o->type)
    {
      case LISPOBJECT_INT:
        snprintf(rv, 100, "%d", o->value_int);
        break;
      case LISPOBJECT_FLOAT:
        snprintf(rv, 100, "%f", o->value_float);
        break;
      case LISPOBJECT_BOOL:
        snprintf(rv, 100, "%s", o->value_bool ? "true" : "false");
        break;
    }
  }

  return rv;
}
