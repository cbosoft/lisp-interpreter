#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "quote"
LispObject_ptr quote(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function quote");
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  return arg->first();
}

LispEnvironmentRow quote_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&quote, "(quote obj)", true)),
  .lfunc = NULL
};
