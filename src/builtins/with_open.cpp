#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "with-open"

// (with-open "path" "mode" name &rest body)
LispObject_ptr with_open(LispList_ptr arg, LispEnvironment_ptr env)
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
  const char* path_cstr = path_atom->get_value_string().c_str();

  LispObject_ptr mode_obj = arg->next();
  if (mode_obj->get_type() != LISPOBJECT_ATOM)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"mode\" should be Atomistic (String): \"r\", \"w\", or \"a\". Got " << mode_obj->repr_type() << ".");

  LispAtom_ptr mode_atom = mode_obj->get_value_atom();
  if (mode_atom->get_type() != LISPATOM_STRING)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"mode\" should be Atomistic (String): \"r\", \"w\", or \"a\". Got " << mode_atom->repr_type() << ".");

  std::string mode_str = mode_atom->get_value_string();
  if (mode_str.size() != 1)
    throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"mode\" should be Atomistic (String): \"r\", \"w\", or \"a\". Got " << mode_atom->repr() << ".");
  char mode_ch = mode_str[0];
  int oflags = 0;

  switch (mode_ch) {
    case 'r':
      oflags = O_RDONLY;
      break;

    case 'a':
      oflags = O_APPEND;
      // fall through
    case 'w':
      oflags |= O_WRONLY;
      break;
  }


  LispObject_ptr name_obj = arg->next();
  if (name_obj->get_type() != LISPOBJECT_SYMBOL)
    throw TypeError(Formatter() << "In " << FUNC << ": Argument \"name\" should be a Symbol. Got " << name_obj->repr_type() << ".");

  LispSymbol_ptr name_symb = name_obj->get_value_symbol();
  int fd = open(path_cstr, oflags);
  if (fd < 0)
    throw IOError(Formatter() << "Could not open file \"" << path_atom->get_value_string() << "\" (" << errno << ") " << strerror(errno));

  LispEnvironment_ptr subenv = std::make_shared<LispEnvironment>(LispEnvironment(env));
  subenv->add(name_symb->get_name(), std::make_shared<LispObject>(LispObject((long)fd)));

  LispList_ptr body = arg->rest(3);
  LispObject_ptr rv = body->eval_each(subenv);
  close(fd);
  return rv;
}
