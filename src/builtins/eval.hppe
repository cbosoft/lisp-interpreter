#pragma once
#include "../types.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"


class LispFunc_eval : public virtual LispBuiltin {

  private:
    inline static const std::string name = "eval";
    inline static const std::string doc =
      "(eval obj)";

  public:
    LispFunc_eval()
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
      narg_check(arg, 1, this->repr(), "obj");

      LispObject_ptr obj = arg->next(true);
      return obj->eval(env);
    }

};
