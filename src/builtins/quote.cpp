#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

LispObject_ptr quote(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function quote");
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) throw ArgumentError(Formatter() << "Wrong number of arguments supplied. In quote: got " << nargs << ".");

  return arg->first();
}
