#pragma once
#include "../types.hpp"
#include "../exception.hpp"
#include "../formatter.hpp"
#include "../exception_check.hpp"


class LispFunc_doc : public virtual LispBuiltin {
  private:
    inline static const std::string name = "doc";
    inline static const std::string doc = 
      "(doc symbol)\n\
      Returns the documentation for function represented in environment by SYMBOL.";

  public:
    LispFunc_doc()
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
      LispObject_ptr symbol_obj = arg->next(true);
      type_check_one(symbol_obj, LISPOBJECT_SYMBOL, this->repr(), "obj");

      LispObject_ptr value_obj;
      LispFunction_ptr value_lfunc;
      LispBuiltin_ptr value_bfunc;
      std::string symbol_name = symbol_obj->get_value_symbol()->get_name();
      LispEnvironment_Type type = env->get(symbol_name, &value_obj, &value_bfunc, &value_lfunc);
      debug_message("after get");

      switch (type) {

        case LISPENV_OBJ:
          return std::make_shared<LispObject>(value_obj->get_doc());

        case LISPENV_BFUNC:
          return std::make_shared<LispObject>(value_bfunc->get_doc());

        case LISPENV_LFUNC:
          return std::make_shared<LispObject>(value_lfunc->get_doc());

        default:
        case LISPENV_NOTFOUND:
          debug_message("not found");
          throw EnvironmentError(Formatter() << "symbol \"" << symbol_name << "\" not found in environment.", symbol_obj->repr_source());

      }

    }
};

