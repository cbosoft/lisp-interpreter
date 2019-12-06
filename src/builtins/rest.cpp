#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "rest"

// (rest list) -> list[1:]
LispObject_ptr rest(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");


  LispObject_ptr list_arg = arg->next(true);
  LispList_ptr list_var = list_arg->get_value_list();
  return std::make_shared<LispObject>(LispObject(list_var->rest()));
}
LispBuiltin_ptr rest_obj = std::make_shared<LispBuiltin>(LispBuiltin(&rest));
