#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

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
      type_check_one(left, LISPOBJECT_ATOM, this->name, "left");
      LispAtom_ptr left_atom = left->get_value_atom();

      LispObject_ptr right = arg->next();
      type_check_one(right, LISPOBJECT_ATOM, this->name, "right");
      LispAtom_ptr right_atom = right->get_value_atom();

      return make_ptr(LispObject(left_atom->multiply(right_atom)));
    }
};