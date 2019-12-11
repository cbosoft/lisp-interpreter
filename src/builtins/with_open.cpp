#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "with-open"

// (with-open "path" "mode" name &rest body)
LispObject_ptr with_open(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check_min(arg, 4, FUNC, "path mode name &rest body");

  LispObject_ptr path_obj = arg->next(true);
  type_check_atom(path_obj, LISPATOM_STRING, FUNC, "path");
  LispAtom_ptr path_atom = path_obj->get_value_atom();
  const char* path_cstr = path_atom->get_value_string().c_str();

  LispObject_ptr mode_obj = arg->next();
  type_check_atom(mode_obj, LISPATOM_STRING, FUNC, "mode");
  LispAtom_ptr mode_atom = mode_obj->get_value_atom();
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
      oflags |= O_WRONLY;
      oflags |= O_CREAT;
      break;

    case 'w':
      oflags = O_TRUNC;
      oflags |= O_WRONLY;
      oflags |= O_CREAT;
      break;
  }

  LispObject_ptr name_obj = arg->next();
  type_check_one(name_obj, LISPOBJECT_SYMBOL, FUNC, "name");

  LispSymbol_ptr name_symb = name_obj->get_value_symbol();
  int fd = open(path_cstr, oflags, S_IRUSR | S_IWUSR);
  if (fd < 0)
    throw IOError(Formatter() << "Could not open file \"" << path_atom->get_value_string() << "\" (" << errno << ") " << strerror(errno));

  LispEnvironment_ptr subenv = make_ptr(LispEnvironment(env, true));
  subenv->add(name_symb->get_name(), make_ptr(LispObject((long)fd)));

  LispList_ptr body = arg->rest(3);
  LispObject_ptr rv = body->eval_each(subenv);
  close(fd);
  return rv;
}


LispEnvironmentRow with_open_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&with_open, "(with-open path mode name &rest body)\nOpens a file at PATH with mode MODE (\"r\" \"w\" \"a\"). The resulting fildes is saved as a variable in a sub environment with name NAME. BODY is evaluated with this subenvironment, afterwards file is closed.", false)),
  .lfunc = NULL
};
