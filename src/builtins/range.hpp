#pragma once
#include "../types.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"


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
      type_check(lower_arg, LISPATOM_INT, this->repr(), "lower");

      LispObject_ptr upper_arg = arg->next();
      type_check(upper_arg, LISPATOM_INT, this->repr(), "upper");

      int lower = lower_arg->get_value_atom()->get_value_int();
      int upper = upper_arg->get_value_atom()->get_value_int();

      LispList_ptr range = std::make_shared<LispList>();
      for (int i = lower; i < upper; i++)
        range->append(std::make_shared<LispObject>(i));

      return std::make_shared<LispObject>(range);
    }
};
