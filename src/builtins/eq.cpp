#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "equal"

LispObject_ptr eq(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 2) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 2.");

  LispObject_ptr left = arg->next(true);
  if (left->get_type() != LISPOBJECT_ATOM) throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"left\" should be Atomistic. Got " << left->repr_type() << ".");
  LispObject_ptr right = arg->next();
  if (right->get_type() != LISPOBJECT_ATOM) throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"right\" should be Atomistic. Got " << right->repr_type() << ".");

  LispAtom_ptr left_atom = left->get_value_atom();
  LispAtom_ptr right_atom = right->get_value_atom();
  return std::make_shared<LispObject>(LispObject(left_atom->eq(right_atom)));
}
