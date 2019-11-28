#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gc.h"
#include "types.h"
#include "atom.h"
#include "exception.h"
#include "debug.h"
#include "parse.h"

extern LispAtom t_atom;


//
LispAtom *LispAtom_new(long *value_int, double *value_float, char *value_string)
{
  LispAtom *rv = calloc(1, sizeof(LispAtom));
  ASSERT_OR_ERROR(rv != NULL, "MemoryError", "LispAtom_new", NULL, NULL, "Error allocating memory for new object");
  
  if (value_int != NULL) {
    rv->value_int = malloc(sizeof(long));
    (*(rv->value_int)) = (*(value_int));
  }

  if (value_float != NULL) {
    rv->value_float = malloc(sizeof(double));
    (*(rv->value_float)) = (*(value_float));
  }

  if (value_string != NULL) {
    rv->value_string = strdup(value_string);
  }

  return rv;
}




// Get string representation of atom
char *LispAtom_repr(LispAtom *a)
{
  char *rv = NULL;

  if (a == &t_atom)
    return "t";

  if (a->value_int) {
    rv = calloc(51, sizeof(char));
    snprintf(rv, 50, "%ld", *a->value_int);
    return rv;
  }

  if (a->value_float) {
    rv = calloc(51, sizeof(char));
    snprintf(rv, 50, "%f", *a->value_float);
    return rv;
  }

  if (a->value_string) {
    rv = calloc(strlen(a->value_string) + 3, sizeof(char));
    snprintf(rv, 50, "\"%s\"", a->value_string);
    return rv;
  }
  
  return "nil";
}




//
int LispAtom_is_truthy(LispAtom *a)
{
  if (a->value_int != NULL)
    return (*a->value_int) != 0;

  if (a->value_float != NULL)
    return (*a->value_float) != 0.0;

  return (a->value_string != NULL);
}




LispAtom *LispAtom_cast_as(LispAtom *a, int type)
{
  int current_type = -1;
  if (a->value_int != NULL) {
    current_type = LISPATOM_INT;
  }
  else if (a->value_float != NULL) {
    current_type = LISPATOM_FLOAT;
  }
  else if (a->value_string != NULL) {
    current_type = LISPATOM_STRING;
  }

  if (current_type == -1) {
    Exception_raise("TypeError", "LispAtom_cast_as", NULL, "Tried to cast nil atom.");
    return NULL;
  }

  // existing type is desired type
  if ((current_type == type) || ((current_type == LISPATOM_INT) && (type == LISPATOM_NUMBER)) || ((current_type == LISPATOM_FLOAT) && (type == LISPATOM_NUMBER))) {
    return LispAtom_new(a->value_int, a->value_float, a->value_string);
  }

  // int to float, float to int
  if ((current_type == LISPATOM_FLOAT) && (type == LISPATOM_INT)) {
    long val = (long)(*a->value_float);
    return LispAtom_new(&val, NULL, NULL);
  }
  else if ((current_type == LISPATOM_INT) && (type == LISPATOM_FLOAT)) {
    double val = (double)(*a->value_int);
    return LispAtom_new(NULL, &val, NULL);
  }

  // string to number
  else if ((current_type == LISPATOM_STRING) && (type == LISPATOM_NUMBER)) {
    int is_int = parser_string_is_int(a->value_string), is_float = parser_string_is_float(a->value_string);
    ASSERT_OR_ERROR(is_int || is_float, "CastError", "LispAtom_cast_as", NULL, NULL, "Cannot cast string %s as Number (Int or Float).", LispAtom_repr(a));
    if (is_int) {
      long val = atol(a->value_string);
      return LispAtom_new(&val, NULL, NULL);
    }
    else {
      double val = atol(a->value_string);
      return LispAtom_new(NULL, &val, NULL);
    }
  }
  else if ((current_type == LISPATOM_STRING) && (type == LISPATOM_INT)) {
    ASSERT_OR_ERROR(parser_string_is_int(a->value_string), "CastError", "LispAtom_cast_as", NULL, NULL, "Cannot cast string %s as Int.", LispAtom_repr(a));
    long val = atol(a->value_string);
    return LispAtom_new(&val, NULL, NULL);
  }
  else if ((current_type == LISPATOM_STRING) && (type == LISPATOM_FLOAT)) {
    ASSERT_OR_ERROR(parser_string_is_float(a->value_string), "CastError", "LispAtom_cast_as", NULL, NULL, "Cannot cast string %s as Float.", LispAtom_repr(a));
    double val = atol(a->value_string);
    return LispAtom_new(NULL, &val, NULL);
  }

  // number to string
  else if ((current_type == LISPATOM_FLOAT) && (type == LISPATOM_STRING)) {
    char *rv = calloc(51, sizeof(char));
    snprintf(rv, 50, "%f", (*a->value_float));
    return LispAtom_new(NULL, NULL, rv);
  }
  else if ((current_type == LISPATOM_INT) && (type == LISPATOM_STRING)) {
    char *rv = calloc(51, sizeof(char));
    snprintf(rv, 50, "%ld", (*a->value_int));
    return LispAtom_new(NULL, NULL, rv);
  }

  Exception_raise("CodeShouldNeverBeReachedError", "LispAtom_cast_as", NULL, "SOMETHING SLIPPED THROUGH THE CRACKS.");
  return NULL;
}




//
void LispAtom_cast_together(LispAtom *left, LispAtom *right, LispAtom **cleft, LispAtom **cright)
{

  // both the same, return the same
  if (left->value_int != NULL && right->value_int != NULL) {
    (*cleft) = LispAtom_new(left->value_int, NULL, NULL);
    (*cright) = LispAtom_new(right->value_int, NULL, NULL);
    return;
  }
  else if (left->value_float != NULL && right->value_float != NULL) {
    (*cleft) = LispAtom_new(NULL, left->value_float, NULL);
    (*cright) = LispAtom_new(NULL, right->value_float, NULL);
    return;
  }
  else if (left->value_string != NULL && right->value_string != NULL) {
    (*cleft) = LispAtom_new(NULL, NULL, left->value_string);
    (*cright) = LispAtom_new(NULL, NULL, right->value_string);
    return;
  }

  // int and float, cast together as two floats
  else if (
      (left->value_int != NULL && right->value_float != NULL) || 
      (left->value_float != NULL && right->value_float != NULL)
      ) {
    (*cleft) = LispAtom_new(NULL, left->value_float, NULL);
    (*cleft) = LispAtom_cast_as(left, LISPATOM_FLOAT);
    (*cright) = LispAtom_cast_as(right, LISPATOM_FLOAT);
    return;
  }

  // string and otherwise, cast together as strings
  else {
    debug_message("Casting together as string\n");
    (*cleft) = LispAtom_cast_as(left, LISPATOM_STRING);
    (*cright) = LispAtom_cast_as(right, LISPATOM_STRING);
  }
}




// Compare atoms
// Two atoms' numerical values are compared. Implemented as a macro so all 
// five can be quickly defined. TODO: compare non-numerical values. Compare 
// lists? Compare strings?
#define NUMERICAL_COMPARE(OPERATOR) \
  ASSERT_OR_ERROR(\
      (left->value_int != NULL) || (left->value_float != NULL),\
      "TypeError", "LispAtom_numerical_compare", NULL, -1, "Left operand: comparison only defined for numbers (Int, Float): got %s.", left->value_string != NULL ? "String" : "nil" );\
  ASSERT_OR_ERROR(\
      (right->value_int != NULL) || (right->value_float != NULL),\
      "TypeError", "LispAtom_numerical_compare", NULL, -1, "Right operand: comparison only defined for numbers (Int, Float): got %s.", right->value_string != NULL ? "String" : "nil" );\
  if ((left->value_int != NULL) && (right->value_int != NULL)) {\
      return (*left->value_int) OPERATOR (*right->value_int);\
  }\
  else if ((left->value_float != NULL) && (right->value_float != NULL)) {\
    debug_message("(comparing floats)");\
    return (*left->value_float) OPERATOR (*right->value_float);\
  }\
  else if ((left->value_int != NULL) && (right->value_float != NULL)) {\
    debug_message("(comparing floats)");\
    return ((double)(*left->value_int)) OPERATOR (*right->value_float);\
  }\
  else {\
    return (*left->value_float) OPERATOR ((double)(*right->value_int));\
  }
int LispAtom_gt(LispAtom *left, LispAtom *right){ NUMERICAL_COMPARE(>); }
int LispAtom_ge(LispAtom *left, LispAtom *right){ NUMERICAL_COMPARE(>=); }
int LispAtom_lt(LispAtom *left, LispAtom *right){ NUMERICAL_COMPARE(<); }
int LispAtom_le(LispAtom *left, LispAtom *right){ NUMERICAL_COMPARE(<=); }
int LispAtom_eq(LispAtom *left, LispAtom *right)
{
  LispAtom *cleft = NULL, *cright = NULL;
  LispAtom_cast_together(left, right, &cleft, &cright);
  if (Exception_check()) return -1;

  if (cleft->value_string != NULL && cright->value_string != NULL) {
    return (strcmp(cleft->value_string, cright->value_string) == 0);
  }

  // a string cast with anything results in strings, therefore they 
  // must both be numbers
  NUMERICAL_COMPARE(==);
}
