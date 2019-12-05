#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "if"

// (if cond value-true value-else)
LispObject_ptr lisp_if(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 3) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 3.");


  LispObject_ptr condition = arg->next(true);
  LispObject_ptr value_true = arg->next();
  LispObject_ptr value_else = arg->next();

  if (condition->is_truthy()) {
    return value_true->eval(env);
  }
  else {
    return value_else->eval(env);
  }
}