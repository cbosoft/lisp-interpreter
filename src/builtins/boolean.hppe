#pragma once
#include "../types.hpp"
#include "../util/formatter.hpp"
#include "../util/debug.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../object/singletons.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"


class LispFunc_or : public virtual LispBuiltin {
  private:
    inline static const std::string name = "or";
    inline static const std::string doc =
      "(or left right)";

  public:

    LispFunc_or()
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
      narg_check(arg, 2, this->repr(), "left right");
      LispObject_ptr left = arg->next(true);
      LispObject_ptr right = arg->next();
      bool rv = left->is_truthy() || right->is_truthy();
      return std::make_shared<LispObject>(rv);
    }
};

class LispFunc_and : public virtual LispBuiltin {
  private:
    inline static const std::string name = "and";
    inline static const std::string doc =
      "(and left right)";

  public:

    LispFunc_and()
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
      narg_check(arg, 2, this->repr(), "left right");
      LispObject_ptr left = arg->next(true);
      LispObject_ptr right = arg->next();
      bool rv = left->is_truthy() && right->is_truthy();
      return std::make_shared<LispObject>(rv);
    }
};

class LispFunc_not : public virtual LispBuiltin {
  private:
    inline static const std::string name = "not";
    inline static const std::string doc =
      "(not val)";

  public:

    LispFunc_not()
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
      narg_check(arg, 1, this->repr(), "val");
      LispObject_ptr val = arg->next(true);
      bool rv = !val->is_truthy();
      return std::make_shared<LispObject>(rv);
    }
};
