#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"




#define FUNC "defvar"
LispObject_ptr defvar(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function defvar");

  narg_check(arg, 2, FUNC, "name value");
 
  LispObject_ptr name = arg->next(true);
  type_check_one(name, LISPOBJECT_SYMBOL, FUNC, "name");

  LispObject_ptr value_raw = arg->next();
  LispObject_ptr value = value_raw->eval(env);
  env->add_something(name->get_value_symbol()->get_name(), value, NULL, NULL);
  return name;
}

LispEnvironmentRow defvar_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&defvar, "(defvar name value)\nDefines a variable NAME with value VALUE.", true)),
  .lfunc = NULL
};
