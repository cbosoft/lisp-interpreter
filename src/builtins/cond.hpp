#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../singletons.hpp"


class LispFunc_cond : public virtual LispBuiltin {
  private:
    inline static const std::string name = "cond";
    inline static const std::string doc = 
      "(cond &rest (condition body) )";

  public:

    LispFunc_cond()
    {
      this->set_macro();
    }

    const std::string repr() const
    {
      return "Macro(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      narg_check_min(arg, 1, this->repr(), "&rest (condition body)");
      LispObject_ptr result;
      for (auto it = arg->begin(); it != arg->end(); ++it) {
    
        if ((*it)->get_type() != LISPOBJECT_LIST) 
          throw TypeError(Formatter() << "cond expects Lists as argument: got " << (*it)->repr_type() << ".");
    
        LispList_ptr cond_then = (*it)->get_value_list();
        LispObject_ptr condition = cond_then->next(true)->eval(env);
        LispObject_ptr then = cond_then->next();
    
        if (condition->is_truthy(env)) {
          return then->eval(env);
        }
      }
    
      return nil;
    }
};
