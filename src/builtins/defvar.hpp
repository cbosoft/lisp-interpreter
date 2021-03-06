#pragma once
#include "../types.hpp"
#include "../util/debug.hpp"
#include "../util/formatter.hpp"
#include "../util/exception_check.hpp"
#include "../list/list.hpp"
#include "../object/object.hpp"
#include "../env/env.hpp"
#include "../symbol/symbol.hpp"
#include "builtin.hpp"


class LispFunc_defvar : public virtual LispBuiltin {
  private:
    inline static const std::string name = "defvar";
    inline static const std::string doc =
      "(defvar name value)\n\
      Define a variable in the current environment scope with NAME and VALUE. Returns\n\
      NAME.";

  public:
    LispFunc_defvar()
    {
      this->set_macro();
    }

    const std::string repr() const
    {
      return "Macro(defvar,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      narg_check(arg, 2, this->name, "name value");
     
      LispObject_ptr name = arg->next(true);
      type_check(name, LISPOBJECT_SYMBOL, this->name, "name");
    
      LispObject_ptr value_raw = arg->next();
      LispObject_ptr value = value_raw->eval(env);
      env->add_something(name->get_value_symbol()->get_name(), value, NULL, NULL);
      return name;
    }
};
