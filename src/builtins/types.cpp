#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"



#define FUNC "list?"
LispObject_ptr is_list(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  LispObject_ptr obj = arg->next(true);
  return make_ptr(LispObject(obj->get_type() == LISPOBJECT_LIST));
}

LispEnvironmentRow is_list_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&is_list, "(list? obj)\nReturns t if OBJ is List, else nil.", false)),
  .lfunc = NULL
};




#undef FUNC
#define FUNC "symbol?"
LispObject_ptr is_symbol(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  LispObject_ptr obj = arg->next(true);
  return make_ptr(LispObject(obj->get_type() == LISPOBJECT_SYMBOL));
}

LispEnvironmentRow is_symbol_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&is_symbol, "(symbol? obj)\nReturns t if OBJ is Symbol, else nil.", false)),
  .lfunc = NULL
};




#undef FUNC
#define FUNC "atom?"
LispObject_ptr is_atom(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  LispObject_ptr obj = arg->next(true);
  return make_ptr(LispObject(obj->get_type() == LISPOBJECT_ATOM));
}

LispEnvironmentRow is_atom_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&is_atom, "(atom? obj)\nReturns t if OBJ is Atom, else nil.", false)),
  .lfunc = NULL
};




#undef FUNC
#define FUNC "int?"
LispObject_ptr is_int(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  LispObject_ptr obj = arg->next(true);
  if (obj->get_type() != LISPOBJECT_ATOM)
    return make_ptr(LispObject(false));

  LispAtom_ptr val = obj->get_value_atom();
  return make_ptr(LispObject(val->get_type() == LISPATOM_INT));
}

LispEnvironmentRow is_int_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&is_int, "(int? obj)\nReturns t if OBJ is Integer, else nil.", false)),
  .lfunc = NULL
};





#undef FUNC
#define FUNC "float?"
LispObject_ptr is_float(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  LispObject_ptr obj = arg->next(true);
  if (obj->get_type() != LISPOBJECT_ATOM)
    return make_ptr(LispObject(false));

  LispAtom_ptr val = obj->get_value_atom();
  return make_ptr(LispObject(val->get_type() == LISPATOM_FLOAT));
}

LispEnvironmentRow is_float_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&is_float, "(float? obj)\nReturns t if OBJ is Float, else nil.", false)),
  .lfunc = NULL
};





#undef FUNC
#define FUNC "string?"
LispObject_ptr is_string(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check(arg, 1, FUNC, "obj");

  LispObject_ptr obj = arg->next(true);
  if (obj->get_type() != LISPOBJECT_ATOM)
    return make_ptr(LispObject(false));

  LispAtom_ptr val = obj->get_value_atom();
  return make_ptr(LispObject(val->get_type() == LISPATOM_STRING));
}

LispEnvironmentRow is_string_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&is_int, "(string? obj)\nReturns t if OBJ is String, else nil.", false)),
  .lfunc = NULL
};
