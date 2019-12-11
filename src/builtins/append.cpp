#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../pointer.hpp"

#define FUNC "append"

LispObject_ptr append(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs < 2) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least 2.");

  LispObject_ptr list_obj = arg->next(true);
  if (list_obj->get_type() != LISPOBJECT_LIST)
    throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"list\" should be a List, got: " << list_obj->repr_type() << ".");

  debug_message(Formatter() << "before copy in old list");
  LispList_ptr rv_list = std::make_shared<LispList>(LispList()), in_list = list_obj->get_value_list();
  for (auto it = in_list->begin(); it != in_list->end(); ++it) {
    rv_list->append((*it));
  }
  debug_message(Formatter() << "after copy in old list");
  for (auto it = ++arg->begin(); it != arg->end(); ++it) {
    rv_list->append((*it));
  }
  debug_message(Formatter() << "after copy in new elements");
  return std::make_shared<LispObject>(LispObject(rv_list));
}

LispEnvironmentRow append_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&append, "(append list val)", false)),
  .lfunc = NULL
};
