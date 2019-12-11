#include <string>
#include <sstream>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "print"

extern LispObject nil;

// (print obj ...)
LispObject_ptr print(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs < 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least 1.");

  std::stringstream ss;
  auto it = arg->begin();
  ss << (*it)->str();
  ++it;
  for (; it != arg->end(); ++it) {
    ss << (*it)->str();
  }

  std::cout << ss.str() << std::endl;

  return make_ptr(nil);
}

LispEnvironmentRow print_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&print, "(print &rest obj)", false)),
  .lfunc = NULL
};
