#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "print"

extern LispObject nil;

// (print obj)
LispObject_ptr print(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");

  LispObject_ptr obj = arg->first();
  obj->print();

  return std::make_shared<LispObject>(nil);
}
