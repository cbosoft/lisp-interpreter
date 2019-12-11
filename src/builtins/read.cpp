#include <sstream>
#include <unistd.h>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "read"

LispObject_ptr lisp_read(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 1) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 1 (\"fd\").");

  LispObject_ptr fd_obj = arg->next(true);
  if (fd_obj->get_type() != LISPOBJECT_ATOM)
    throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"fd\" should be Atomistic (Int). Got " << fd_obj->repr_type() << ".");

  LispAtom_ptr fd_atom = fd_obj->get_value_atom();
  if (fd_atom->get_type() != LISPATOM_INT)
    throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"fd\" should be Atomistic (Int). Got " << fd_atom->repr_type() << ".");

  int fd = (int)fd_atom->get_value_int(), rv = 0;
  std::stringstream ss;
  char cbuf[10] = {0};

  while ( (rv = read(fd, cbuf, 9)) > 0 ) {
    ss << cbuf;
    memset(cbuf, 0, 10);
  }

  if (rv < 0)
    throw IOError(Formatter() << "Error reading file descriptor " << fd << " (" << errno << ") " << strerror(errno));

  return make_ptr(LispObject(ss.str()));
}

LispEnvironmentRow lisp_read_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&lisp_read, "(read list)", false)),
  .lfunc = NULL
};
