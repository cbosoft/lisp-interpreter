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


class LispFunc_multiply : public virtual LispBuiltin {
  private:
    inline static const std::string name = "multiply";
    inline static const std::string doc = 
      "(multiply left right)\n\
      Multiply the left and right operands, returning the result.";

  public:
    LispFunc_multiply()
    {
    }

    const std::string repr() const
    {
      return "Func(multiply,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 2, this->name, "left right");

      LispObject_ptr left = arg->next(true);
      type_check(left, LISPOBJECT_ATOM, this->name, "left");
      LispAtom_ptr left_atom = left->get_value_atom();

      LispObject_ptr right = arg->next();
      type_check(right, LISPOBJECT_ATOM, this->name, "right");
      LispAtom_ptr right_atom = right->get_value_atom();

      return std::make_shared<LispObject>(left_atom->multiply(right_atom));
    }
};
