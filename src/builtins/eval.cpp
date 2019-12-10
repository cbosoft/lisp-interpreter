#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "eval"

// (eval obj)
LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");

  LispObject_ptr obj = arg->next(true);
  return obj->eval(env);
}

