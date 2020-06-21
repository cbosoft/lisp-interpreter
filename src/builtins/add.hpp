#pragma once
#include "../types.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "../atom/atom.hpp"
#include "builtin.hpp"





class LispFunc_add : public virtual LispBuiltin {
  private:
    inline static const std::string name = "add";
    inline static const std::string doc =
      "(add left right)\n\
      Adds the left and right operands returning the result. If either of the operands\n\
      are Strings, concatenation is performed instead.";

  public:
    LispFunc_add() 
    {
    }

    const std::string repr() const
    {
      return "Func(add,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
    
      narg_check(arg, 2, this->repr(), "left right");
    
      LispObject_ptr left = arg->next(true);
      type_check_one(left, LISPOBJECT_ATOM, this->repr(), "left");
    
      LispObject_ptr right = arg->next();
      type_check_one(right, LISPOBJECT_ATOM, this->repr(), "right");
    
      LispAtom_ptr left_atom = left->get_value_atom();
      LispAtom_ptr right_atom = right->get_value_atom();
      return std::make_shared<LispObject>(left_atom->add(right_atom));
    }
};
