#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../builtins.hpp"




// Create an entry in the local environment's function table.
// Usage: (defvar name arglist &rest body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            rest of the arguments make up the list of contents in the function body
LispObject_ptr defvar(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function defvar");

  int nargs = arg->count();
  if (nargs != 2) throw ArgumentError(Formatter() << "Wrong number of arguments supplied. In defvar: got " << nargs << ", expected 2.");
 
  LispObject_ptr name = arg->next(true);
  if (name->get_type() != LISPOBJECT_SYMBOL) throw std::runtime_error(Formatter() << "Argument has wrong type: name should be a Symbol, not " << name->repr_type() << ".");

  LispObject_ptr value = arg->next()->eval(env);
  env->add_something(name->get_value_symbol()->get_name(), value, NULL, NULL);
  return name;
}
LispBuiltin_ptr defvar_obj = std::make_shared<LispBuiltin>(LispBuiltin(&defvar, true));
