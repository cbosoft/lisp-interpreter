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


class LispFunc_modulo : public virtual LispBuiltin {
  private:
    inline static const std::string name = "modulo";
    inline static const std::string doc = 
      "(modulo left right)\n\
      Returns LEFT modulo RIGHT.";

  public:
    LispFunc_modulo()
    {
    }

    const std::string repr() const
    {
      return "Func(modulo,builtin)";
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
      type_check(left_obj, LISPOBJECT_ATOM, this->name, "left");
      LispAtom_ptr left_atom = left_obj->get_value_atom();

      LispObject_ptr right_obj = arg->next();
      type_check(right_obj, LISPOBJECT_ATOM, this->name, "right");
      LispAtom_ptr right_atom = right_obj->get_value_atom();

      return std::make_shared<LispObject>(left_atom->modulo(right_atom));
    }
};
