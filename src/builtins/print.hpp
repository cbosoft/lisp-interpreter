#pragma once
#include <string>
#include <sstream>

#include "../types.hpp"
#include "../util/formatter.hpp"
#include "../util/debug.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../object/singletons.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"



class LispFunc_print : public virtual LispBuiltin {
  private:
    inline static const std::string name = "print";
    inline static const std::string doc = 
      "(print &rest objects)";

  public:
    LispFunc_print()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check_min(arg, 1, this->name, "&rest objects");

      std::stringstream ss;
      auto it = arg->begin();
      ss << (*it)->str();
      ++it;
      for (; it != arg->end(); ++it) {
        ss << (*it)->str();
      }

      std::cout << ss.str() << std::endl;
      return nil;
    }
};
