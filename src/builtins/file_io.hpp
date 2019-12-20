#pragma once
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"


class LispFunc_with_open : public virtual LispBuiltin {
  private:
    inline static const std::string name = "with-open";
    inline static const std::string doc = 
      "(with-open path mode name &rest body)\n\
      Opens file at PATH with MODE as fildes NAME and executes BODY.\n\
      PATH should be a string. MODE should be one of \"r\", \"w\" or \"a\" for read, \n\
      write, or append modes..";
  public:
    LispFunc_with_open()
    {
      this->set_macro();
    }

    const std::string repr() const
    {
      return "Macro(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check_min(arg, 4, this->repr(), "path mode name &rest body");

      LispObject_ptr path_obj = arg->next(true);
      type_check_atom(path_obj, LISPATOM_STRING, this->repr(), "path");
      LispAtom_ptr path_atom = path_obj->get_value_atom();
      std::string path_str = path_atom->get_value_string();
      const char *path_cstr = path_str.c_str();

      LispObject_ptr mode_obj = arg->next();
      type_check_atom(mode_obj, LISPATOM_STRING, this->repr(), "mode");
      LispAtom_ptr mode_atom = mode_obj->get_value_atom();
      std::string mode_str = mode_atom->get_value_string();
      debug_message(Formatter() << this->repr() << "  " << mode_str << "  " << path_str << "  " << path_cstr);
      if (mode_str.size() != 1)
        throw ArgumentError(Formatter() << "In " << this->repr() << ": Argument \"mode\" should be Atomistic (String): \"r\", \"w\", or \"a\". Got " << mode_atom->repr() << ".");
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

        default:
          throw ArgumentError(Formatter() << "In " << this->name << ": Argument \"mode\" should be Atomistic (String): \"r\", \"w\", or \"a\". Got " << mode_atom->repr() << ".");
      }
    
      LispObject_ptr name_obj = arg->next();
      type_check_one(name_obj, LISPOBJECT_SYMBOL, this->name, "name");
    
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
};

class LispFunc_write : public virtual LispBuiltin{
  private:
    inline static const std::string name = "write";
    inline static const std::string doc = 
      "(write fd &rest objs)\n\
      Writes the repr of OBJS in sequence to the file described by FD, which is an \n\
      integer file descriptor.";

  public:

    LispFunc_write()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check_min(arg, 2, this->name, "fd &rest values");

      LispObject_ptr fd_obj = arg->next(true);
      type_check_atom(fd_obj, LISPATOM_INT, this->name, "fd");
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
          throw IOError(Formatter() << "Error writing to fd " << fd << " (" << errno << ") " << strerror(errno));
      }

      return make_ptr(LispObject(tot_written.str()));
    }
};


class LispFunc_read : public virtual LispBuiltin {
  private:
    inline static const std::string name = "write";
    inline static const std::string doc = 
      "(read fd)\n\
      Read until EOF from FD, returning the result as a String.";

  public:

    LispFunc_read()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 1, this->name, "fd");

      LispObject_ptr fd_obj = arg->next(true);
      type_check_atom(fd_obj, LISPATOM_INT, this->name, "fd");
      LispAtom_ptr fd_atom = fd_obj->get_value_atom();

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

};
