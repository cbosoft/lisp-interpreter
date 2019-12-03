#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

LispObject_ptr exit(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message("builtin function exit");
  (void) env;

  int nargs = arg->count();
  if (nargs > 1) throw ArgumentError(Formatter() << "Wrong number of arguments supplied. In exit: got " << nargs << ", expected no more than 1.");

  int rc = 0;
  if (nargs == 1) {
    LispObject_ptr rc_obj = arg->first();
    if (rc_obj->get_type() != LISPOBJECT_ATOM) throw ArgumentError(Formatter() << "In exit: Argument \"return-code\" should be Atomistic (Int). Got " << rc_obj->repr_type() << ".");
    LispAtom_ptr rc_atom = rc_obj->get_value_atom();
    if (rc_atom->get_type() != LISPATOM_INT) throw ArgumentError(Formatter() << "In exit: Argument \"return-code\" should be Atomistic (Int). Got " << rc_atom->repr_type() << ".");
    rc = rc_atom->get_value_int();
  }

  exit(rc);
}

