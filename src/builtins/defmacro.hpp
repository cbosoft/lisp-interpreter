#pragma once
#include "../types.hpp"
#include "../util/debug.hpp"
#include "../util/exception_check.hpp"
#include "../util/formatter.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "../symbol/symbol.hpp"
#include "../env/env.hpp"
#include "../function/function.hpp"
#include "builtin.hpp"


class LispFunc_defmacro : LispBuiltin {
  private:
    inline static const std::string name = "defmacro";
    inline static const std::string doc = \
      "(defmacro name list &rest body)\n\
      Creates a new macro with NAME in environment. ARGS is a list of symbols which \n\
      are arguments to the macro. These are NOT evaluated before the macro is run.\n\
      If you want args to be eval'd before runing, use 'defun'.";

  public:
    LispFunc_defmacro()
    {
      this->set_macro();
    }

    const std::string repr() const
    {
      return "Macro(defmacro,builtin)";
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      narg_check_min(arg, 3, this->name, "name args &rest body");

      LispObject_ptr name_obj = arg->next(true);
      type_check(name_obj, LISPOBJECT_SYMBOL, this->name, "name");

      LispObject_ptr arglist_obj = arg->next();
      type_check(arglist_obj, LISPOBJECT_LIST, this->name, "arglist");

      LispList_ptr argnames_list = arglist_obj->get_value_list();
      auto argnames_iter = argnames_list->begin();
      for (; argnames_iter != argnames_list->end(); ++argnames_iter) {
        type_check(*argnames_iter, LISPOBJECT_SYMBOL, this->name, "arg");
      }
    
      std::string name_str = name_obj->get_value_symbol()->get_name();
      LispList_ptr body = arg->rest(2);
      LispFunction_ptr lfunc = std::make_shared<LispFunction>(argnames_list, body, name_str, "TODO");
      env->add_something(name_str, NULL, NULL, lfunc);
      return name_obj;
    }

    const std::string get_doc() const {
      return this->doc;
    }
};
