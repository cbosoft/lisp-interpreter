#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../pointer.hpp"



#define FUNC "defmacro"

LispObject_ptr defmacro(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function defmacro");

  int nargs = arg->count();
  if (nargs < 3) throw ArgumentError(Formatter() << "Wrong number of arguments supplied. In defmacro: got " << nargs << ", expected at least 3.");
 
  auto arg_iter = arg->begin();
  LispObject_ptr name = (*arg_iter);
  if (name->get_type() != LISPOBJECT_SYMBOL) throw TypeError(Formatter() << "Argument has wrong type: name should be a Symbol, not " << name->repr_type() << ".");

  arg_iter++;
  LispObject_ptr arglist = (*arg_iter);
  if (arglist->get_type() != LISPOBJECT_LIST) throw TypeError(Formatter() << "Argument has wrong type: arglist should be a List, not " << arglist->repr_type());

  LispList_ptr argnames_list = arglist->get_value_list();
  auto argnames_iter = argnames_list->begin();
  for (; argnames_iter != argnames_list->end(); ++argnames_iter) {
    if ((*argnames_iter)->get_type() != LISPOBJECT_SYMBOL) throw TypeError(Formatter() << "Argument has wrong type: arglist items should be Symbols, not " << (*argnames_iter)->repr_type());
  }
  debug_message("after arglist_items check");

  std::string name_str = name->get_value_symbol()->get_name();
  LispList_ptr body = arg->rest()->rest();
  LispFunction_ptr lfunc = std::make_shared<LispFunction>(LispFunction(argnames_list, body, name_str, "TODO"));
  env->add_something(name_str, NULL, NULL, lfunc);
  return name;
}

LispEnvironmentRow defmacro_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&defmacro, "(defmacro name arglist &rest body)", true)),
  .lfunc = NULL
};
