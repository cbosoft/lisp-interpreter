#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "object.h"




// Create new string object.
LispObject *new_string_object(char *value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));

  rv->type = LISPOBJECT_STRING;

  rv->value_string = calloc(strlen(value)+1, sizeof(char));
  strcpy(rv->value_string, value);

  return rv;
}




// Create new integer.
LispObject *new_int_object(int value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_INT;
  rv->value_int = value;
  return rv;
}




// Create new float.
LispObject *new_float_object(double value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_FLOAT;
  rv->value_float = value;
  return rv;
}




// Create new boolean.
LispObject *new_bool_object(int value)
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_BOOL;
  rv->value_bool = value;
  return rv;
}




// Create new list object.
// A blank list can have objects added to it using add_object_to_list(LispObject *obj).
LispObject *new_list_object()
{
  LispObject *rv = calloc(1, sizeof(LispObject));
  rv->type = LISPOBJECT_LIST;
  return rv;
}




// Create a new LispObject, inferring type from string $s.
// if it contains only digits 0 to 9, its an integer. If it also contains 'e',
// '.', '+' or '-' then its a float. If its a left parens '(' then its a list.
// If its 'false' or 'true', then its a bool. Otherwise, its a string.
LispObject *new_object_guess_type(char *s) {
  int len = strlen(s), i, ch;

  int not_number=0, not_integer=0;

  if (strcmp(s, "(") == 0)
    return new_list_object();

  if (strcmp(s, "false") == 0)
    return new_bool_object(0);

  if (strcmp(s, "true") == 0)
    return new_bool_object(1);

  for (i = 0, ch=(int)s[0]; i < len; i++, ch=(int)s[i]) {
    if ((ch < (int)'0' || ch > (int)'9')) {
      if (ch != (int)'.' && ch != (int)'e' && ch != (int)'+' && ch != (int)'-') {
        not_integer = 1;
      }
    }
    else {
      not_number = 1;
      break;
    }
  }

  if (not_number)
    return new_string_object(s);

  if (not_integer)
    return new_float_object(atof(s));

  return new_int_object(atoi(s));
}




// Add object $toadd to list $list.
// $list must be a list.
void add_object_to_list(LispObject *list, LispObject *toadd)
{
  assert(list->type == LISPOBJECT_LIST);

  LispObject *i = list->list_child;

  while (i->list_next != NULL) i = i->list_next;
  // now i has the last item in the linked list
  
  i->list_next = toadd;

}




// Remove and return last entry from the list.
// Return value is the only reference to the object in the list, and must be
// free'd after use.
// $list must be a list.
LispObject *pop(LispObject *list)
{
  assert(list->type == LISPOBJECT_LIST);

  LispObject *i = list->list_child, *p, *rv;

  while (i->list_next != NULL) {
    p = i;
    i = i->list_next;
  }
  // now i has the last item in the linked list, and
  // p has the next-to-last.
  
  rv = i;
  p->list_next = NULL;
  return rv;
}
