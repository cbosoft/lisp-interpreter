#pragma once
#include "../types.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

class LispFunc_range : public virtual LispBuiltin {
  private:
    inline static const std::string name = "range";
    inline static const std::string doc = 
      "(range lower upper)\n\
      Returns range of integers between LOWER and (excluding) UPPER.";

  public:
    LispFunc_range()
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
      narg_check(arg, 2, this->repr(), "lower upper");

      LispObject_ptr lower_arg = arg->next(true);
      type_check_atom(lower_arg, LISPATOM_INT, this->repr(), "lower");

      LispObject_ptr upper_arg = arg->next();
      type_check_atom(upper_arg, LISPATOM_INT, this->repr(), "upper");

      int lower = lower_arg->get_value_atom()->get_value_int();
      int upper = upper_arg->get_value_atom()->get_value_int();

      LispList_ptr range = make_ptr(LispList());
      for (int i = lower; i < upper; i++)
        range->append(make_ptr(LispObject(i)));

      return make_ptr(LispObject(range));
    }
};
