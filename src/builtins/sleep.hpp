#pragma once
#include <unistd.h>

#include <thread>

#include "../types.hpp"
#include "../exception_check.hpp"


class LispFunc_sleep : public virtual LispBuiltin {
  private:
    inline static const std::string name = "sleep";
    inline static const std::string doc =
      "(sleep len)";

  public:
    LispFunc_sleep()
    {
    }

    const std::string repr() const
    {
      return "Func(sleep,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;

      narg_check(arg, 1, this->repr(), "len");

      LispObject_ptr len = arg->next(true);
      type_check_atom(len, LISPATOM_INT, this->repr(), "len");

      int l = len->get_value_atom()->get_value_int();

      sleep(l);

      return std::make_shared<LispObject>(l);
    }
};
