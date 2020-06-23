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


class LispFunc_subtract : public virtual LispBuiltin {
  private:
    inline static const std::string name = "subtract";
    inline static const std::string doc = 
      "(subtract left right)\n\
      Subtract the right operand from the left, returning the result.";

  public:
    LispFunc_subtract()
    {
    }

    const std::string repr() const
    {
      return "Func(subtract,builtin)";
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
    
      return std::make_shared<LispObject>(left_atom->subtract(right_atom));
    }
};
