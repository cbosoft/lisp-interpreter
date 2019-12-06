#include <string>
#include <sstream>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

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
  ss << (*it)->repr();
  ++it;
  for (; it != arg->end(); ++it) {
    ss << " " << (*it)->repr();
  }

  std::cout << ss.str() << std::endl;

  return std::make_shared<LispObject>(nil);
}