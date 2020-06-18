#pragma once
#include "../debug.hpp"
#include "../types.hpp"
#include "../formatter.hpp"
#include "../singletons.hpp"
#include "../exception_check.hpp"


class LispFunc_defun : public virtual LispBuiltin {
  private:
    inline static const std::string name = "defun";
    inline static const std::string doc = 
      "(defun name args &optional default-values docstring &rest body)\n\
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
      narg_check_min(arg, 4, this->name, "name args &optional default-values docstring &rest body");
     
      LispObject_ptr name = arg->next(true);
      type_check_one(name, LISPOBJECT_SYMBOL, this->name, "name");
    
      LispObject_ptr arglist = arg->next();
      type_check_one(arglist, LISPOBJECT_LIST, this->name, "args");

      LispList_ptr argnames_list = arglist->get_value_list();
      for (auto argnames_iter = argnames_list->begin(); argnames_iter != argnames_list->end(); ++argnames_iter)
        type_check_one(*argnames_iter, LISPOBJECT_SYMBOL, this->name, "argname");

      LispObject_ptr next = arg->next();
      LispObject_ptr docstring;
      LispList_ptr defvalue_list = std::make_shared<LispList>();
      auto type = next->get_type();
      if (type == LISPOBJECT_LIST) {
        defvalue_list = next->get_value_list();
        if (defvalue_list->count() > argnames_list->count())
          throw ArgumentError(Formatter() << "Default value list cannot be longer than argument list.");

        docstring = arg->next();
      }
      else {
        docstring = next;
      }

      type_check_atom(docstring, LISPATOM_STRING, this->name, "docstring");
      LispAtom_ptr docstring_atom = docstring->get_value_atom();
      std::string docstring_string = docstring_atom->get_value_string();
    
      std::string name_str = name->get_value_symbol()->get_name();
      LispList_ptr body = arg->rest(3);
      LispFunction_ptr lfunc = std::make_shared<LispFunction>(argnames_list, defvalue_list, body, name_str, docstring_string);
      env->add(name_str, lfunc);
      return name;
    }
};
