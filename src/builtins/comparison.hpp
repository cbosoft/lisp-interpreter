#pragma once
#include "../types.hpp"
#include "../util/formatter.hpp"
#include "../util/debug.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../object/object.hpp"
#include "../atom/atom.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"

#include "binary_check.hpp"


class LispFunc_gt : public virtual LispBuiltin {
  private:
    inline static const std::string name = "greater-than?";
    inline static const std::string doc = 
      "(greater-than? left right)\n\
      (> left right)";

  public:
    LispFunc_gt()
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
      BINARY_CHECK;
      return std::make_shared<LispObject>(LispObject(left_atom->gt(right_atom)));
    }
};

class LispFunc_ge : public virtual LispBuiltin {
  private:
    inline static const std::string name = "greater-than-or-equal-to?";
    inline static const std::string doc = 
      "(greater-than-or-equal-to? left right)\n\
      (>= left right)";

  public:
    LispFunc_ge()
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
      BINARY_CHECK;
      return std::make_shared<LispObject>(LispObject(left_atom->ge(right_atom)));
    }
};

class LispFunc_lt : public virtual LispBuiltin {
  private:
    inline static const std::string name = "less-than?";
    inline static const std::string doc = 
      "(less-than? left right)\n\
      (< left right)";

  public:
    LispFunc_lt()
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
      BINARY_CHECK;
      return std::make_shared<LispObject>(LispObject(left_atom->lt(right_atom)));
    }
};

class LispFunc_le : public virtual LispBuiltin {
  private:
    inline static const std::string name = "less-than-or-equal-to?";
    inline static const std::string doc = 
      "(less-than-or-equal-to? left right)\n\
      (<= left right)";

  public:
    LispFunc_le()
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
      BINARY_CHECK;
      return std::make_shared<LispObject>(left_atom->le(right_atom));
    }
};

class LispFunc_eq : public virtual LispBuiltin {
  private:
    inline static const std::string name = "equal?";
    inline static const std::string doc = 
      "(equal? left right)\n\
      (= left right)";

  public:
    LispFunc_eq()
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
      narg_check(arg, 2, this->name, "left right");\
      LispObject_ptr left_obj = arg->next(true);\
      LispObject_ptr right_obj = arg->next();\

      return std::make_shared<LispObject>(left_obj->eq(right_obj));
    }
};
