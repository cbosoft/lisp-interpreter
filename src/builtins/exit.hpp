#pragma once
#include "../types.hpp"
#include "../util/formatter.hpp"
#include "../util/debug.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"


class LispFunc_exit : public virtual LispBuiltin {
  private:
    inline static const std::string name = "exit";
    inline static const std::string doc = 
      "(exit &optional return-code)";

  public:
    LispFunc_exit()
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
      (void) env;

      int nargs = arg->count();
      narg_check_max(arg, 1, this->repr(), "&optional return-code");

      int rc = 0;
      if (nargs == 1) {
        LispObject_ptr rc_obj = arg->first();
        type_check(rc_obj, LISPATOM_INT, this->repr(), "return-code");
        LispAtom_ptr rc_atom = rc_obj->get_value_atom();
        rc = rc_atom->get_value_int();
      }

      exit(rc);
    }
};
