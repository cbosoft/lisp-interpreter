#pragma once
#include "object.h"

typedef struct LispFunction LispFunction;
struct LispFunction{
  LispObject *body;
  int number_required_args;
  //int number_optional_args;
  char **arg_names; // used to create the environment for the function
};
