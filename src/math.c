#include <stdlib.h>

#include "types.h"
#include "debug.h"
#include "exception.h"
#include "list.h"
#include "atom.h"
#include "object.h"

// Numerical operations on two objects
#define NUMERICAL_OPERATION(FUNC, OPERATION)\
  debug_message("BUILTIN FUNCTION "FUNC"\n");\
  int nargs = LispList_count(arg);\
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", FUNC, NULL, NULL, "Function takes 2 arguments (got %d).", nargs);\
  (void) env;\
  LispObject *l = arg->value;\
  LispObject *r = arg->next->value;\
  ASSERT_OR_ERROR(l->type == r->type && l->type == LISPOBJECT_ATOM,\
      "TypeError",\
      FUNC, \
      NULL, NULL, \
      "Function expects Atomistic arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));\
  debug_message(""FUNC" AFTER ATOM CHECK\n");\
  LispAtom *cl = NULL, *cr = NULL;\
  LispAtom \
  *la = LispAtom_cast_as(l->value_atom, LISPATOM_NUMBER),\
  *ra = LispAtom_cast_as(r->value_atom, LISPATOM_NUMBER);\
  if (Exception_check()) { \
    Exception_reset();\
    Exception_raisef("TypeError", FUNC, NULL, "Function expects Number (Int, Float) arguments got %s and %s.", LispObject_type(l), LispObject_type(r));\
    return NULL;\
  }\
  LispAtom_cast_together(la, ra, &cl, &cr);\
  debug_message(""FUNC" AFTER CAST TOGETHER\n");\
  if (cl->value_float != NULL) {\
    LispObject *rv = LispObject_new_float((*cl->value_float) OPERATION (*cr->value_float));\
    return rv;\
  }\
  LispObject *rv = LispObject_new_int((*cl->value_int) OPERATION (*cr->value_int));\
  return rv;
LispObject *add(LispListElement *arg, LispEnvironment *env) { NUMERICAL_OPERATION("add", +) }
LispObject *subtract(LispListElement *arg, LispEnvironment *env) { NUMERICAL_OPERATION("subtract", -) }
LispObject *multiply(LispListElement *arg, LispEnvironment *env) { NUMERICAL_OPERATION("multiply", *) }
LispObject *divide(LispListElement *arg, LispEnvironment *env) { 
  debug_message("BUILTIN FUNCTION divide\n");
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "divide", NULL, NULL, "Function takes 2 arguments (got %d).", nargs);
  (void) env;
  LispObject *l = arg->value;
  LispObject *r = arg->next->value;
  ASSERT_OR_ERROR(l->type == r->type && l->type == LISPOBJECT_ATOM,
      "TypeError",
      "divide", 
      NULL, NULL, 
      "Function expects Atomistic arguments, got %s and %s.", LispObject_type(l), LispObject_type(r));
  debug_message("divide AFTER ATOM CHECK\n");
  LispAtom *cl = NULL, *cr = NULL;
  LispAtom 
  *la = LispAtom_cast_as(l->value_atom, LISPATOM_NUMBER),
  *ra = LispAtom_cast_as(r->value_atom, LISPATOM_NUMBER);
  if (Exception_check()) { 
    Exception_reset();
    Exception_raisef("TypeError", "divide", NULL, "Function expects Number (Int, Float) arguments got %s and %s.", LispObject_type(l), LispObject_type(r));
    return NULL;
  }
  LispAtom_cast_together(la, ra, &cl, &cr);
  debug_message("divide AFTER CAST TOGETHER\n");

  ASSERT_OR_ERROR(
      ((cr->value_int != NULL) && ( (*cr->value_int) != 0)) || 
      ((cr->value_float != NULL) && ( (*cr->value_float) != 0.0)), "DivideByZeroError", "divide", NULL, NULL, "Right operand to divide cannot be zero.");
  if (cl->value_float != NULL) {
    LispObject *rv = LispObject_new_float((*cl->value_float) / (*cr->value_float));
    return rv;
  }
  LispObject *rv = LispObject_new_int((*cl->value_int) / (*cr->value_int));
  return rv;
}
LispBuiltin add_obj = {&add, LISPBUILTIN_GREEDY};
LispBuiltin subtract_obj = {&subtract, LISPBUILTIN_GREEDY};
LispBuiltin multiply_obj = {&multiply, LISPBUILTIN_GREEDY};
LispBuiltin divide_obj = {&divide, LISPBUILTIN_GREEDY};

