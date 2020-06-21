#pragma once
#include <string>
#include <sstream>

#include "../types.hpp"
#include "../util/formatter.hpp"
#include "../util/debug.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../object/singletons.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"



class LispFunc_throw : public virtual LispBuiltin {
  private:
    inline static const std::string name = "throw";
    inline static const std::string doc =
      "(throw object)";

  public:
    LispFunc_throw()
    {
      this->set_macro();
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
      narg_check(arg, 1, this->name, "object");

      auto exc_obj = arg->next(true);

      throw exc_obj;
    }
};
