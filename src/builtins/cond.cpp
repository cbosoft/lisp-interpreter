#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../pointer.hpp"
#include "../singletons.hpp"

#define FUNC "cond"

// (cond (cond1 value-true1) (cond2 value-true2) ...)
LispObject_ptr cond(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs < 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least 1.");

  LispObject_ptr result;
  for (auto it = arg->begin(); it != arg->end(); ++it) {

    if ((*it)->get_type() != LISPOBJECT_LIST) 
      throw TypeError(Formatter() << "cond expects Lists as argument: got " << (*it)->repr_type() << ".");

    LispList_ptr cond_then = (*it)->get_value_list();
    LispObject_ptr condition = cond_then->next(true);
    LispObject_ptr then = cond_then->next();

    if (condition->is_truthy()) {
      return then->eval(env);
    }
  }

  return std::make_shared<LispObject>(nil);
}

LispEnvironmentRow cond_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&cond, "(cond (cond then) ...)", true)),
  .lfunc = NULL
};
