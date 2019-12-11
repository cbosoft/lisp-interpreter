#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../pointer.hpp"

#define FUNC "import"

extern LispParser parser;

// (import path)
LispObject_ptr import(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1.");

  LispObject_ptr path_obj = arg->next(true);
  int obj_type = path_obj->get_type();
  LispAtom_ptr path_atom;
  LispSymbol_ptr path_symb;
  std::string path;

  switch (obj_type) {

    case LISPOBJECT_ATOM:
      path_atom = path_obj->get_value_atom();
      if (path_atom->get_type() != LISPATOM_STRING) 
        throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"path\" should be String. Got " << path_atom->repr_type() << ".");
      path = path_atom->get_value_string();
      break;

    case LISPOBJECT_SYMBOL:
      path_symb = path_obj->get_value_symbol();
      path = path_symb->get_name();
      break;

    case LISPOBJECT_LIST:
      throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"path\" should be Atomistic (String) or Symbol. Got " << path_obj->repr_type() << ".");

    default:
      throw AuthorError("Unexpected type encountered in import.");
  }

  debug_message(Formatter() << "searching for module \"" << path << "\"");
  LispList_ptr root = parser.parse_module(path);
  return root->eval_each(env);
}

LispEnvironmentRow import_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&import, "(import path)", false)),
  .lfunc = NULL
};
