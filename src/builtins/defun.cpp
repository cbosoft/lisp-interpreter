#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"




// Create an entry in the local environment's function table.
// Usage: (defun name arglist &rest body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            rest of the arguments make up the list of contents in the function body

#define FUNC "defun"
LispObject_ptr defun(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function defun");

  narg_check(arg, 3, FUNC, "name arglist body");
 
  LispObject_ptr name = arg->next(true);
  type_check_one(name, LISPOBJECT_SYMBOL, FUNC, "name");

  LispObject_ptr arglist = arg->next();
  type_check_one(arglist, LISPOBJECT_LIST, FUNC, "arglist");

  LispList_ptr argnames_list = arglist->get_value_list();
  for (auto argnames_iter = argnames_list->begin(); argnames_iter != argnames_list->end(); ++argnames_iter)
    type_check_one(*argnames_iter, LISPOBJECT_SYMBOL, FUNC, "argname");
  debug_message("after arglist_items check");

  std::string name_str = name->get_value_symbol()->get_name();
  LispList_ptr body = arg->rest(2);
  LispFunction_ptr lfunc = make_ptr(LispFunction(argnames_list, body, name_str, "TODO"));
  env->add_something(name_str, NULL, NULL, lfunc);
  return name;
}
LispEnvironmentRow defun_row = {
  .name = "defun",
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&defun, "(defun fname arglist &rest body)\nDefines a function FNAME which takes arguments ARGLIST and has body BODY.", true)),
  .lfunc = NULL
};
