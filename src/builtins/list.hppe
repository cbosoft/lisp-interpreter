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


class LispFunc_list : public virtual LispBuiltin {
  private:
    inline static const std::string name = "list";
    inline static const std::string doc = 
      "(list &rest obj)";

  public:
    LispFunc_list()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      return std::make_shared<LispObject>(arg);
    }
};



class LispFunc_pop : public virtual LispBuiltin {
  private:
    inline static const std::string name = "pop";
    inline static const std::string doc = 
      "(pop list)";

  public:
    LispFunc_pop()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 1, this->repr(), "list");

      LispObject_ptr list_arg = arg->next(true);
      type_check_one(list_arg, LISPOBJECT_LIST, this->repr(), "list");
      LispList_ptr list_var = list_arg->get_value_list();

      if (!list_var->count())
        return std::make_shared<LispObject>(std::make_shared<LispList>());

      return list_var->first();
    }
};




class LispFunc_rest : public virtual LispBuiltin {
  private:
    inline static const std::string name = "rest";
    inline static const std::string doc = 
      "(rest list)";

  public:
    LispFunc_rest()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 1, this->repr(), "list");

      LispObject_ptr list_arg = arg->next(true);
      type_check_one(list_arg, LISPOBJECT_LIST, this->repr(), "list");
      LispList_ptr list_var = list_arg->get_value_list();

      if (!list_var->count())
        return std::make_shared<LispObject>(std::make_shared<LispList>());

      return std::make_shared<LispObject>(list_var->rest());
    }
};
