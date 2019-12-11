#include <sstream>
#include <unistd.h>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

#define FUNC "write"

LispObject_ptr lisp_write(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  narg_check_min(arg, 2, FUNC, "fd &rest values");

  LispObject_ptr fd_obj = arg->next(true);
  type_check_atom(fd_obj, LISPATOM_INT, FUNC, "fd");
  LispAtom_ptr fd_atom = fd_obj->get_value_atom();

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

  return make_ptr(LispObject(tot_written.str()));
}

LispEnvironmentRow lisp_write_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&lisp_write, "(write fd str)\nWrites STR to fildes FD, as returned by a call to (open ...).", false)),
  .lfunc = NULL
};
