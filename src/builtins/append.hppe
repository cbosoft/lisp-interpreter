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


class LispFunc_append : public virtual LispBuiltin {
  private:
    inline static const std::string name = "append";
    inline static const std::string doc = 
      "(append list &rest obj)";
  public:
    LispFunc_append()
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
      narg_check_min(arg, 2, this->repr(), "list &rest obj");
    
      LispObject_ptr list_obj = arg->next(true);
      type_check_one(list_obj, LISPOBJECT_LIST, this->repr(), "list");

      LispList_ptr rv_list = std::make_shared<LispList>(), in_list = list_obj->get_value_list();
      for (auto it = in_list->begin(); it != in_list->end(); ++it) {
        rv_list->append((*it));
      }

      for (auto it = ++arg->begin(); it != arg->end(); ++it) {
        rv_list->append((*it));
      }
      return std::make_shared<LispObject>(rv_list);
    }
};
