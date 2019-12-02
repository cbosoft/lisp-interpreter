#include "debug.hpp"
#include "types.hpp"
#include "formatter.hpp"




// Create an entry in the local environment's function table.
// Usage: (define name arglist &rest body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            rest of the arguments make up the list of contents in the function body
LispObject *define(LispList *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DEFINE");

  int nargs = arg->count();
  if (nargs < 3) throw std::runtime_error(Formatter() << "Wring number of arguments supplied. In define: got " << nargs << ".");
 
  auto arg_iter = arg->begin();
  LispObject *name = (*arg_iter);
  if (name->get_type() != LISPOBJECT_SYMBOL) throw std::runtime_error(Formatter() << "Argument has wrong type: name should be a Symbol, not " << name->repr_type() << ".");

  arg_iter++;
  LispObject *arglist = (*arg_iter);
  if (arglist->get_type() != LISPOBJECT_LIST) throw std::runtime_error(Formatter() << "Argument has wrong type: arglist should be a List, not " << arglist->repr_type());

  LispList *argnames_list = arglist->get_value_list();
  auto argnames_iter = argnames_list->begin();
  for (; argnames_iter != argnames_list->end(); ++argnames_iter) {
    if ((*argnames_iter)->get_type() != LISPOBJECT_SYMBOL) throw std::runtime_error(Formatter() << "Argument has wrong type: arglist items should be Symbols, not " << (*argnames_iter)->repr_type());
  }
  debug_message("AFTER ARGLIST_ITEMS CHECK");

  LispList *body = arg->rest()->rest();
  LispFunction *lfunc = new LispFunction(argnames_list, new LispObject(body));
  env->add_something(name->get_value_symbol()->get_name(), NULL, NULL, lfunc);
  return name;
}
