#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

LispObject_ptr exit(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function exit");
  (void) env;

  exit(1);

  int nargs = arg->count();
  if (nargs > 1) throw ArgumentError(Formatter() << "Wrong number of arguments supplied. In exit: got " << nargs << ", expected no more than 1.");

  LispObject_ptr rc_obj = arg->first();
  if (rc_obj->get_type() != LISPOBJECT_ATOM) throw ArgumentError(Formatter() << "Argument \"return-code\" should be Atomistic (Int). In exit: got " << nargs << ", expected no more than 1.");

  return arg->first();
}

