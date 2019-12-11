#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../pointer.hpp"

#define FUNC "eval-file"

extern LispParser parser;

// (eval-file path)
LispObject_ptr eval_file(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");

  LispObject_ptr path_obj = arg->next(true);
  if (path_obj->get_type() != LISPOBJECT_ATOM) throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"path\" should be Atomistic (String). Got " << path_obj->repr_type() << ".");
  LispAtom_ptr path_atom = path_obj->get_value_atom();
  if (path_atom->get_type() != LISPATOM_STRING) throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"path\" should be String. Got " << path_atom->repr_type() << ".");

  LispList_ptr root = parser.parse_file(path_atom->get_value_string());
  return root->eval_each(env);
}

LispEnvironmentRow eval_file_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&eval_file, "(eval-file path)", false)),
  .lfunc = NULL
};
