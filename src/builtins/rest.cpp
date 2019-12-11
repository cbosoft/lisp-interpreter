#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "rest"

// (rest list) -> list[1:]
LispObject_ptr rest(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "list");

  LispObject_ptr list_arg = arg->next(true);
  type_check_one(list_arg, LISPOBJECT_LIST, FUNC, "list");
  LispList_ptr list_var = list_arg->get_value_list();
  return std::make_shared<LispObject>(LispObject(list_var->rest()));
}

LispEnvironmentRow rest_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&rest, "(rest list)", false)),
  .lfunc = NULL
};
