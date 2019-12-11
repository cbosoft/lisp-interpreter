#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "subtract"

LispObject_ptr subtract(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 2, FUNC, "left right");

  LispObject_ptr left = arg->next(true);
  type_check_one(left, LISPOBJECT_ATOM, FUNC, "left");
  LispAtom_ptr left_atom = left->get_value_atom();

  LispObject_ptr right = arg->next();
  type_check_one(right, LISPOBJECT_ATOM, FUNC, "right");
  LispAtom_ptr right_atom = right->get_value_atom();

  return make_ptr(LispObject(left_atom->subtract(right_atom)));
}

LispEnvironmentRow subtract_row = {
  .name = "subtract",
  .alias = "-",
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&subtract, "(subtract left right)", false)),
  .lfunc = NULL
};
