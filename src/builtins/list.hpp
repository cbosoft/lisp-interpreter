#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

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
      return make_ptr(LispObject(arg));
    }
};
