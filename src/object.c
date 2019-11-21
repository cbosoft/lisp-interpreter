#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "debug.h"
#include "types.h"
#include "list.h"
#include "gc.h"




// Create new string object.
LispObject *LispObject_new_nil()
{
  LispObject *rv = calloc(1, sizeof(LispObject));

  assert_or_error(rv != NULL, "LispObject_new", "Error allocating memory for new object");

  return rv;
}




// Create new string object.
LispObject *LispObject_new_string(char *value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;

  rv->type = LISPOBJECT_STRING;

  rv->value_string = calloc(strlen(value)+1, sizeof(char));
  strcpy(rv->value_string, value);

  return rv;
}




// Create new integer.
LispObject *LispObject_new_int(int value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;
  rv->type = LISPOBJECT_INT;
  rv->value_int = value;
  return rv;
}




// Create new float.
LispObject *LispObject_new_float(double value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;
  rv->type = LISPOBJECT_FLOAT;
  rv->value_float = value;
  return rv;
}




// Create new boolean.
LispObject *LispObject_new_bool(int value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;
  rv->type = LISPOBJECT_BOOL;
  rv->value_bool = value;
  return rv;
}




// Create new symbol.
LispObject *LispObject_new_symbol(char *name)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;
  rv->type = LISPOBJECT_SYMBOL;
  rv->symbol_name = strdup(name);
  return rv;
}




// Create new list object.
// A blank list can have objects added to it using LispObject_add_object_to_list(LispObject *obj).
LispObject *LispObject_new_list()
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;
  rv->type = LISPOBJECT_LIST;
  rv->value_list = LispList_new_element();
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

  if ((s[0] == '"' && s[len-1] == '"') ||
      (s[0] == '\'' && s[len-1] == '\'')) {
    debug_message("GUESSING %s is STRING\n", s);
    s[len-1] = '\0';
    s++;
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




// Print the value of $o, recursively Note: does not print a newline at end, so
// user should call "LispObject_print" (no trailing underscore) which calls this
// function, and adds the required newline.
void LispObject_print_(LispObject *o) { 

  LispListElement *list_iter = NULL; 
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
      
      fprintf(fp, "(");
      for(list_iter = o->value_list; list_iter->value != NULL; list_iter = list_iter->next) {
        LispObject_print_(list_iter->value);

        if (list_iter->next->value != NULL)
          fprintf(fp, " ");
      }

      fprintf(stdout, ")");
      break;

  }
}

void LispObject_print(LispObject *o)
{
  LispObject_print_(o);
  fprintf(stdout, "\n");
}




// make "nil" (zero-value) object
void LispObject_make_nil(LispObject *o)
{
  if (o->value_string != NULL) {
    free(o->value_string);
    o->value_string = NULL;
  }

  o->value_int = 0;
  o->value_float = 0.0;
  o->value_bool = 0;
  o->value_list = NULL;

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
      dest->value_list = source->value_list;
      break;
  }

  dest->type = source->type;
}




// Get string representation of type of object $o
char *LispObject_type(LispObject *o)
{
  assert_or_error(o != NULL, "LispObject_type", "Can't get type of NULL object");
  ERROR_CHECK;

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




// Represent object as a string
char *LispObject_repr(LispObject *o)
{
  char *rv;
  if (o->type == LISPOBJECT_STRING) {
    rv = calloc(strlen(o->value_string)+3, sizeof(char));
    sprintf(rv, "'%s'", o->value_string);
  }
  else if (o->type == LISPOBJECT_SYMBOL) {
    rv = strdup(o->symbol_name);
  }
  else if (o->type == LISPOBJECT_LIST) {
    //TODO
    rv = "(LIST REPR NOT IMPLEMENTED)";
  }
  else {

    rv = calloc(100, sizeof(char));

    switch (o->type) {

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




// Check if the object is "truthy": if it is an empty list, or zero then it is
// false. Otherwise, it is true.
int LispObject_is_truthy(LispObject *o)
{
  int rv = -1;

  switch (o->type) {

    case LISPOBJECT_INT:
      rv = o->value_int != 0;
      break;

    case LISPOBJECT_FLOAT:
      debug_message("(interpreting float as bool)\n");
      rv = o->value_float != 0.0;
      break;

    case LISPOBJECT_BOOL:
      rv = o->value_bool;
      break;

    case LISPOBJECT_STRING:
      rv = o->value_string != NULL;
      break;

    case LISPOBJECT_SYMBOL:
      Exception_raise("LispObject_is_truthy", "Cannot discern \"truthy-ness\" of symbol");
      break;
  }

  return rv;
}




// Compare objects
// Two objects' numerical values are compared. Implemented as a macro so all 
// five can be quickly defined. TODO: compare non-numerical values. Compare 
// lists? Compare strings?
#define NUMERICAL_COMPARE(L,R,OPERATOR) \
  assert_or_error(\
      (left->type == LISPOBJECT_FLOAT || left->type == LISPOBJECT_INT) &&\
      (right->type == LISPOBJECT_FLOAT || right->type == LISPOBJECT_INT),\
      "LispObject_numerical_compare", "numerical comparison not defined for non-numerical values.");\
  if (Exception_check()) return -1;\
  if (L->type == R->type) {\
    if (L->type == LISPOBJECT_INT) {\
      return L->value_int OPERATOR R->value_int;\
    }\
    else {\
      debug_message("(comparing floats)");\
      return L->value_float OPERATOR R->value_float;\
    }\
  }\
  else {\
    debug_message("(comparing floats)");\
    if (L->type == LISPOBJECT_FLOAT) {\
      return L->value_float OPERATOR ((float)R->value_int);\
    }\
    else {\
      return ((float)L->value_int) OPERATOR R->value_float;\
    }\
  }
int LispObject_gt(LispObject *left, LispObject *right){ NUMERICAL_COMPARE(left, right, >); }
int LispObject_ge(LispObject *left, LispObject *right){ NUMERICAL_COMPARE(left, right, >=); }
int LispObject_lt(LispObject *left, LispObject *right){ NUMERICAL_COMPARE(left, right, <); }
int LispObject_le(LispObject *left, LispObject *right){ NUMERICAL_COMPARE(left, right, <=); }
int LispObject_eq(LispObject *left, LispObject *right)
{
  assert_or_error( 
      (left->type == right->type) ||
      ((left->type == LISPOBJECT_FLOAT) && (right->type == LISPOBJECT_INT)) ||
      ((right->type == LISPOBJECT_FLOAT) && (left->type == LISPOBJECT_INT)),
      "LispObject_equal", "comparison operands must both be the same type, or at least both numbers.");

  switch (left->type) {
    case LISPOBJECT_BOOL:
      return left->value_bool == right->value_bool;

    case LISPOBJECT_STRING:
      return strcmp(left->value_string, right->value_string) == 0;

    case LISPOBJECT_SYMBOL:
      return strcmp(left->symbol_name, right->symbol_name) == 0;
  }

  // must be numbers
  NUMERICAL_COMPARE(left, right, ==);
}
