#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../builtins.hpp"
#include "../pointer.hpp"

#define FUNC "add"

LispObject_ptr add(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 2, FUNC, "left right");

  LispObject_ptr left = arg->next(true);
  type_check_one(left, LISPOBJECT_ATOM, FUNC, "left");

  LispObject_ptr right = arg->next();
  type_check_one(right, LISPOBJECT_ATOM, FUNC, "right");

  LispAtom_ptr left_atom = left->get_value_atom();
  LispAtom_ptr right_atom = right->get_value_atom();
  return make_ptr(LispObject(left_atom->add(right_atom)));
}

LispEnvironmentRow add_row = {
  .name = "add",
  .alias = "+",
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&add, "(add left right)", false)),
  .lfunc = NULL
};
