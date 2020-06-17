#pragma once
#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

class LispFunc_defun : public virtual LispBuiltin {
  private:
    inline static const std::string name = "defun";
    inline static const std::string doc = 
      "(defun name list &rest body)\n\
      Creates a new function with NAME in environment. ARGS is a list of symbols which \n\
      are arguments to the function. These ARE evaluated before the function is run.\n\
      If you do not want the arguments to be eval'd, use 'defmacro'.";

  public:
    LispFunc_defun()
    {
      this->set_macro();
    }

    const std::string repr() const
    {
      return "Macro(defun,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      narg_check_min(arg, 3, this->name, "name arglist &rest body");
     
      LispObject_ptr name = arg->next(true);
      type_check_one(name, LISPOBJECT_SYMBOL, this->name, "name");
    
      LispObject_ptr arglist = arg->next();
      type_check_one(arglist, LISPOBJECT_LIST, this->name, "arglist");
    
      LispList_ptr argnames_list = arglist->get_value_list();
      for (auto argnames_iter = argnames_list->begin(); argnames_iter != argnames_list->end(); ++argnames_iter)
        type_check_one(*argnames_iter, LISPOBJECT_SYMBOL, this->name, "argname");
    
      std::string name_str = name->get_value_symbol()->get_name();
      LispList_ptr body = arg->rest(2);
      LispFunction_ptr lfunc = make_ptr(LispFunction(argnames_list, body, name_str, "TODO"));
      env->add_something(name_str, NULL, NULL, lfunc);
      return name;
    }
};
