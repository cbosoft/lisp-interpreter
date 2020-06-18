#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"


class LispFunc_if : public virtual LispBuiltin {
  private:
    inline static const std::string name = "if";
    inline static const std::string doc = 
      "(if condition value-true value-else)";

  public:
    LispFunc_if()
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
      narg_check(arg, 3, this->repr(), "cond value-true value-false");

      LispObject_ptr condition = arg->next(true)->eval(env);
      LispObject_ptr value_true = arg->next();
      LispObject_ptr value_else = arg->next();

      if (condition->is_truthy(env))
        return value_true->eval(env);

      return value_else->eval(env);
    }
};
