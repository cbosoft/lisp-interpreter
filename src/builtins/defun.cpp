#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../builtins.hpp"




// Create an entry in the local environment's function table.
// Usage: (defun name arglist &rest body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            rest of the arguments make up the list of contents in the function body
LispObject_ptr defun(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function defun");

  int nargs = arg->count();
  if (nargs < 3) throw ArgumentError(Formatter() << "Wrong number of arguments supplied. In defun: got " << nargs << ", expected at least 3.");
 
  auto arg_iter = arg->begin();
  LispObject_ptr name = (*arg_iter);
  if (name->get_type() != LISPOBJECT_SYMBOL) throw TypeError(Formatter() << "Argument has wrong type: name should be a Symbol, not " << name->repr_type() << ".");

  arg_iter++;
  LispObject_ptr arglist = (*arg_iter);
  if (arglist->get_type() != LISPOBJECT_LIST) TypeError(Formatter() << "Argument has wrong type: arglist should be a List, not " << arglist->repr_type());

  LispList_ptr argnames_list = arglist->get_value_list();
  auto argnames_iter = argnames_list->begin();
  for (; argnames_iter != argnames_list->end(); ++argnames_iter) {
    if ((*argnames_iter)->get_type() != LISPOBJECT_SYMBOL) throw TypeError(Formatter() << "Argument has wrong type: arglist items should be Symbols, not " << (*argnames_iter)->repr_type());
  }
  debug_message("after arglist_items check");

  LispList_ptr body = arg->rest()->rest();
  LispFunction_ptr lfunc = std::make_shared<LispFunction>(LispFunction(argnames_list, std::make_shared<LispObject>(LispObject(body))));
  env->add_something(name->get_value_symbol()->get_name(), NULL, NULL, lfunc);
  return name;
}