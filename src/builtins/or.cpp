#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "or"

extern LispObject nil;

// (or value-1 value-2)
LispObject_ptr lisp_or(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 2) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 2.");


  LispObject_ptr left = arg->next(true);
  LispObject_ptr right = arg->next();
  bool rv = left->is_truthy() || right->is_truthy();
  return std::make_shared<LispObject>(LispObject(rv));
}

