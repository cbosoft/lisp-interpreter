#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "list"

LispObject_ptr list(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs < 1) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least 1.");
  narg_check_min(arg, 1, FUNC, "&rest elements");

  return make_ptr(LispObject(arg));
}

LispEnvironmentRow list_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&list, "(list &rest elements)", false)),
  .lfunc = NULL
};
