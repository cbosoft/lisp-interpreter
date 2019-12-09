#include <sstream>
#include <unistd.h>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"

#define FUNC "write"

LispObject_ptr lisp_write(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs < 2) 
    throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected at least 2 (fd &rest values).");

  LispObject_ptr fd_obj = arg->next(true);
  if (fd_obj->get_type() != LISPOBJECT_ATOM)
    throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"fd\" should be Atomistic (Int). Got " << fd_obj->repr_type() << ".");

  LispAtom_ptr fd_atom = fd_obj->get_value_atom();
  if (fd_atom->get_type() != LISPATOM_INT)
    throw ArgumentError(Formatter() << "In " << FUNC << ": Argument \"fd\" should be Atomistic (Int). Got " << fd_atom->repr_type() << ".");

  int fd = (int)fd_atom->get_value_int(), rv = 0;
  std::stringstream tot_written;
  std::string s;

  for (auto it = ++arg->begin(); it != arg->end(); ++it) {
    s = (*it)->str();
    tot_written << s;
    const char *cbuf = s.c_str();
    rv = write(fd, cbuf, strlen(cbuf));
    if (rv < 0)
      throw IOError(Formatter() << "Error writing to fildes " << fd << " (" << errno << ") " << strerror(errno));
  }

  return std::make_shared<LispObject>(LispObject(tot_written.str()));
}
