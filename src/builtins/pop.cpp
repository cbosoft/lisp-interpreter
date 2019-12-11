#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "pop"

// (pop list) -> list[0]
LispObject_ptr pop(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");


  LispObject_ptr list_arg = arg->next(true);
  LispList_ptr list_var = list_arg->get_value_list();
  return list_var->first();
}

LispEnvironmentRow pop_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&pop, "(pop list)", false)),
  .lfunc = NULL
};
