#include "exception.hpp"
#include "types.hpp"
#include "formatter.hpp"

void type_check_any(LispObject_ptr obj, std::list<LispObject_Type> types, std::string in, std::string arg)
{
  bool passing = false;
  for (auto & type : types) {
    if (obj->get_type() == type) passing = true;
    break;
  }


  if (not passing) {
    std::stringstream ss;
    ss << "( ";
    for (auto & type : types) {
      ss << obj->repr_type(type) << " ";
    }
    ss << ")";
    throw TypeError(Formatter() << "In " << in << ": Argument \"" << arg << "\" should be " << ss.str() << ". Got " << obj->repr_type() << ".");
  }
}


void type_check_one(LispObject_ptr obj, LispObject_Type type, std::string in, std::string arg)
{
  if (obj->get_type() != type)
    throw TypeError(Formatter() << "In " << in << ": Argument \"" << arg << "\" should be " << obj->repr_type(type) << ". Got " << obj->repr_type() << ".");
}

void type_check_atom(LispObject_ptr obj, LispAtom_Type type, std::string in, std::string arg)
{
  if (obj->get_type() != LISPOBJECT_ATOM)
    throw TypeError(Formatter() << "In " << in << ": Argument \"" << arg << "\" should be " << LispAtom::repr_type(type) << ". Got " << obj->repr_type() << ".");

  LispAtom_Type atom_type;
  if ((atom_type = obj->get_value_atom()->get_type()) != type)
    throw TypeError(Formatter() << "In " << in << ": Argument \"" << arg << "\" should be " << LispAtom::repr_type(type) << ". Got " << LispAtom::repr_type(atom_type) << ".");
}

void type_check_symbol(LispObject_ptr obj, LispEnvironment_Type type, std::string in, std::string arg, LispEnvironment_ptr env)
{
  if (obj->get_type() != LISPOBJECT_SYMBOL)
    throw TypeError(Formatter() << "In " << in << ": Argument \"" << arg << "\" should be " << LispEnvironment::repr_type(type) << ". Got " << obj->repr_type() << ".");

  LispEnvironment_Type symtype = env->get(obj, nullptr, nullptr, nullptr);
  if (symtype != type)
    throw TypeError(Formatter() << "In " << in << ": Argument \"" << arg << "\" should be " << LispEnvironment::repr_type(type) << ". Got " << LispEnvironment::repr_type(symtype) << ".");
}

void narg_check(LispList_ptr arg, int expected_nargs, std::string in, std::string argnames)
{
  int nargs = arg->count();
  if (nargs != expected_nargs)
    throw ArgumentError(Formatter() << "In " << in << ": Wrong number of arguments supplied. Got " << nargs << ", expected " << expected_nargs << " (" << argnames << ").");
}

void narg_check_min(LispList_ptr arg, int min_nargs, std::string in, std::string argnames)
{
  int nargs = arg->count();
  if (nargs < min_nargs)
    throw ArgumentError(Formatter() << "In " << in << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least " << min_nargs << " (" << argnames << ").");
}

void narg_check_max(LispList_ptr arg, int max_nargs, std::string in, std::string argnames)
{
  int nargs = arg->count();
  if (nargs > max_nargs)
    throw ArgumentError(Formatter() << "In " << in << ": Wrong number of arguments supplied. Got " << nargs << ", expected at most " << max_nargs << " (" << argnames << ").");
}
