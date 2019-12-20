#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

class LispFunc_divide : public virtual LispBuiltin {
  private:
    inline static const std::string name = "divide";
    inline static const std::string doc = 
      "(divide left right)\n\
      Divides left operand by right, returning the result.";

  public:
    LispFunc_divide() 
    {
    }

    const std::string repr() const
    {
      return "Func(divide,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 2, this->name, "left right");

      LispObject_ptr left_obj = arg->next(true);
      type_check_one(left_obj, LISPOBJECT_ATOM, this->name, "left");
      LispAtom_ptr left_atom = left_obj->get_value_atom();

      LispObject_ptr right_obj = arg->next();
      type_check_one(right_obj, LISPOBJECT_ATOM, this->name, "right");
      LispAtom_ptr right_atom = right_obj->get_value_atom();

      return std::make_shared<LispObject>(LispObject(left_atom->divide(right_atom)));
    }
};
