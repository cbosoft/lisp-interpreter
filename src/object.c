#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gc.h"
#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "debug.h"
#include "types.h"
#include "list.h"
#include "atom.h"




// Create new string object.
LispObject *LispObject_new_nil()
{
  LispObject *rv = calloc(1, sizeof(LispObject));

  ASSERT_OR_ERROR(rv != NULL, "MemoryError", "LispObject_new", NULL, NULL, "Error allocating memory for new object");

  return rv;
}




// Create new string object.
LispObject *LispObject_new_string(char *value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;

  rv->value_atom = LispAtom_new(NULL, NULL, value);
  ERROR_CHECK;

  rv->type = LISPOBJECT_ATOM;

  return rv;
}




// Create new integer.
LispObject *LispObject_new_int(long value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;

  rv->value_atom = LispAtom_new(&value, NULL, NULL);
  ERROR_CHECK;

  rv->type = LISPOBJECT_ATOM;

  return rv;
}




// Create new float.
LispObject *LispObject_new_float(double value)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;

  rv->value_atom = LispAtom_new(NULL, &value, NULL);
  ERROR_CHECK;

  rv->type = LISPOBJECT_ATOM;

  return rv;
}




// Create new symbol.
LispObject *LispObject_new_symbol(char *name)
{
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;

  rv->type = LISPOBJECT_SYMBOL;
  rv->value_symbol = strdup(name);
  return rv;
}




// Create new list object.
// A blank list can have objects added to it using LispObject_add_object_to_list(LispObject *obj).
LispObject *LispObject_new_list()
{
  debug_message("new list\n");
  LispObject *rv = LispObject_new_nil();
  ERROR_CHECK;

  debug_message("after error check\n");
  rv->type = LISPOBJECT_LIST;
  rv->value_list = LispList_new_element();
  return rv;
}




// Print the value of $o, recursively Note: does not print a newline at end, so
// user should call "LispObject_print" (no trailing underscore) which calls this
// function, and adds the required newline.
void LispObject_print_(LispObject *o) { 

  LispListElement *list_iter = NULL; 
  FILE *fp = stdout;
  char *repr = NULL;

  switch (o->type) {

    case LISPOBJECT_LIST:
      
      fprintf(fp, "(");
      for(list_iter = o->value_list; list_iter->value != NULL; list_iter = list_iter->next) {
        LispObject_print_(list_iter->value);

        if (list_iter->next->value != NULL)
          fprintf(fp, " ");
      }

      fprintf(stdout, ")");
      break;

   default:
      repr = LispObject_repr(o);
      fprintf(fp, "%s", repr);
      break;

  }
}

void LispObject_print(LispObject *o)
{
  LispObject_print_(o);
  fprintf(stdout, "\n");
}




// Get string representation of type of object $o
char *LispObject_type(LispObject *o)
{
  ASSERT_OR_ERROR(o != NULL, "NullException", "LispObject_type", NULL, NULL, "Object is null.");

  switch (o->type)
  {
    case LISPOBJECT_SYMBOL:
      return "Symbol";

    case LISPOBJECT_ATOM:
      return "Atom";

    case LISPOBJECT_LIST:
      return "List";

    case LISPOBJECT_NIL:
      return "Nil";
  }

  return "Unkown object";
}




// Represent object as a string
char *LispObject_repr(LispObject *o)
{
  debug_message("REPR\n");

  switch (o->type) {

    case LISPOBJECT_NIL:
      return "nil";

    case LISPOBJECT_ATOM:
      return LispAtom_repr(o->value_atom);

    case LISPOBJECT_SYMBOL:
      return o->value_symbol;

    case LISPOBJECT_LIST:
      return "(LIST REPR NOT IMPLEMENTED)";

    default:
      Exception_raise("Exception", "LispObject_repr", NULL, "Unknown type encountered in repr.");
      return NULL;
  }
}




// Check if the object is "truthy": if it is an empty list, or zero then it is
// false. Otherwise, it is true.
int LispObject_is_truthy(LispObject *o)
{

  switch (o->type) {

    case LISPOBJECT_ATOM:
      return LispAtom_is_truthy(o->value_atom);

    case LISPOBJECT_SYMBOL:
      Exception_raise("Exception", "LispObject_is_truthy", o, "\"truthy-ness\" of symbol is ambiguous.");
      return -1;

    case LISPOBJECT_NIL:
      return 0;

    case LISPOBJECT_LIST:
      return LispList_count(o->value_list) > 0;

    default:
      Exception_raise("Exception", "LispObject_is_truthy", o, "\"truthy-ness\" of unknown object is ambiguous.");
      return -1;
  }

}




// Compare objects
// Two objects' numerical values are compared. Implemented as a macro so all 
// five can be quickly defined. TODO: compare non-numerical values. Compare 
// lists? Compare strings?
#define COMPARE(ATOM_OPNAME) \
  ASSERT_OR_ERROR(\
      (left->type == LISPOBJECT_ATOM),\
      "TypeError", "LispObject_numerical_compare", left, -1, "Left operand: comparison only defined for Atoms (Int, Float, String): got %s.", LispObject_type(left));\
  ASSERT_OR_ERROR(\
      (right->type == LISPOBJECT_ATOM),\
      "TypeError", "LispObject_numerical_compare", right, -1, "Right operand: comparison only defined for Atoms (Int, Float, String): got %s.", LispObject_type(right));\
      return ATOM_OPNAME(left->value_atom, right->value_atom);
int LispObject_gt(LispObject *left, LispObject *right){ COMPARE(LispAtom_gt) }
int LispObject_ge(LispObject *left, LispObject *right){ COMPARE(LispAtom_ge) }
int LispObject_lt(LispObject *left, LispObject *right){ COMPARE(LispAtom_lt) }
int LispObject_le(LispObject *left, LispObject *right){ COMPARE(LispAtom_le) }
int LispObject_eq(LispObject *left, LispObject *right)
{
  ASSERT_OR_ERROR( 
      (left->type == right->type) && (left->type != LISPOBJECT_LIST),
      "TypeError", "LispObject_equal", NULL, -1, "comparison operands must both be the same type, and not a List: got %s and %s.", LispObject_type(left), LispObject_type(right));

  if (left->type == LISPOBJECT_SYMBOL)
    return strcmp(left->value_symbol, right->value_symbol) == 0;

  // must be numbers
  COMPARE(LispAtom_eq);
}




// boolean operations
#define BOOLEAN(OPERATOR) \
  return LispObject_is_truthy(left) OPERATOR LispObject_is_truthy(right);
int LispObject_or(LispObject *left, LispObject *right){ BOOLEAN(||); }
int LispObject_and(LispObject *left, LispObject *right){ BOOLEAN(&&); }




// return value of atom as specified type, or nah
double LispObject_as_float(LispObject *o)
{
  ASSERT_OR_ERROR( o->type == LISPOBJECT_ATOM && ((o->value_atom->value_int != NULL)  || (o->value_atom->value_float != NULL)), "TypeError", "LispObject_as_float", o, 0.0, "Object has no value as atom.");
  double rv = 0.0;
  if (o->value_atom->value_float != NULL) {
    rv = (*(o->value_atom->value_float));
  }
  else {
    rv = (double)(*(o->value_atom->value_int));
  }

  return rv;
}
