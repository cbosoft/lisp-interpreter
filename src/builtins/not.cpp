#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "not"

extern LispObject nil;

// (not value)
LispObject_ptr lisp_not(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");


  LispObject_ptr operand = arg->next(true);
  bool rv = !operand->is_truthy();
  return std::make_shared<LispObject>(LispObject(rv));
}
