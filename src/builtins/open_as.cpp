#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "while_open"

LispObject_ptr while_open(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs < 3) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least 4 (\"path\", \"mode\", \"name\", \"body\")).");

  LispObject_ptr path_obj = arg->next(true);
  if (path_obj->get_type() != LISPOBJECT_ATOM)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"path\" should be Atomistic (String). Got " << path_obj->repr_type() << ".");

  LispAtom_ptr path_atom = path_obj->get_value_atom();
  if (path_atom->get_type() != LISPATOM_STRING)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"path\" should be Atomistic (String). Got " << path_atom->repr_type() << ".");

  LispObject_ptr mode_obj = arg->next(true);
  if (mode_obj->get_type() != LISPOBJECT_ATOM)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"mode\" should be Atomistic (String). Got " << mode_obj->repr_type() << ".");

  LispAtom_ptr mode_atom = mode_obj->get_value_atom();
  if (mode_atom->get_type() != LISPATOM_STRING)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"mode\" should be Atomistic (String). Got " << mode_atom->repr_type() << ".");

  std::string mode_str = mode_atom->get_value_string();
  // TODO for each char in str, append mode
  std::ios_base::openmode mode;
  switch (mode_ch) {
    case 'w':
      mode = std::ios::app;
      break;
    case 'r':
      mode = std::ios::;
      break;
    case 'a':
      mode = std::ios::app;
      break;
  }
  

  LispObject_ptr name_obj = arg->next();
  if (path_obj->get_type() != LISPOBJECT_SYMBOL)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"name\" should be a Symbol. Got " << path_obj->repr_type() << ".");

  LispSymbol_ptr name_symb = name_obj->get_value_symbol();

  LispList_ptr body = arg->rest()->rest();

  LispFileIO fp(name_symb->get_name(), std::);


  return 
}
