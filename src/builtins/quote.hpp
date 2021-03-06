#pragma once
#include "../types.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"


class LispFunc_quote : public virtual LispBuiltin {
  private:
    inline static const std::string name = "quote";
    inline static const std::string doc = 
      "(quote obj)";

  public:
    LispFunc_quote()
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

      narg_check(arg, 1, this->repr(), "obj");

      return arg->first();
    }
};
