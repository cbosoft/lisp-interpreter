#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"


class LispFunc_is_list : public virtual LispBuiltin {
  private:
    inline static const std::string name = "list?";
    inline static const std::string doc = 
      "(list? obj)";

  public:

    LispFunc_is_list()
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
    narg_check(arg, 1, this->repr(), "obj");
    LispObject_ptr obj = arg->next(true);
    return make_ptr(LispObject(obj->get_type() == LISPOBJECT_LIST));
  }
};




class LispFunc_is_symbol : public virtual LispBuiltin {
  private:
    inline static const std::string name = "symbol?";
    inline static const std::string doc = 
      "(symbol? obj)";

  public:

    LispFunc_is_symbol()
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
    narg_check(arg, 1, this->repr(), "obj");
    LispObject_ptr obj = arg->next(true);
    return make_ptr(LispObject(obj->get_type() == LISPOBJECT_SYMBOL));
  }
};



class LispFunc_is_atom : public virtual LispBuiltin {
  private:
    inline static const std::string name = "atom?";
    inline static const std::string doc = 
      "(atom? obj)";

  public:

    LispFunc_is_atom()
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
    narg_check(arg, 1, this->repr(), "obj");
    LispObject_ptr obj = arg->next(true);
    return make_ptr(LispObject(obj->get_type() == LISPOBJECT_ATOM));
  }
};



class LispFunc_is_int : public virtual LispBuiltin {
  private:
    inline static const std::string name = "int?";
    inline static const std::string doc = 
      "(int? obj)";

  public:

    LispFunc_is_int()
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
    narg_check(arg, 1, this->repr(), "obj");
    LispObject_ptr obj = arg->next(true);
    if (obj->get_type() != LISPOBJECT_ATOM)
      return make_ptr(LispObject(false));

    LispAtom_ptr val = obj->get_value_atom();
    return make_ptr(LispObject(val->get_type() == LISPATOM_INT));
  }
};



class LispFunc_is_float : public virtual LispBuiltin {
  private:
    inline static const std::string name = "float?";
    inline static const std::string doc = 
      "(float? obj)";

  public:

    LispFunc_is_float()
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
    narg_check(arg, 1, this->repr(), "obj");
    LispObject_ptr obj = arg->next(true);
    if (obj->get_type() != LISPOBJECT_ATOM)
      return make_ptr(LispObject(false));

    LispAtom_ptr val = obj->get_value_atom();
    return make_ptr(LispObject(val->get_type() == LISPATOM_FLOAT));
  }
};



class LispFunc_is_string : public virtual LispBuiltin {
  private:
    inline static const std::string name = "string?";
    inline static const std::string doc = 
      "(string? obj)";

  public:

    LispFunc_is_string()
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
    narg_check(arg, 1, this->repr(), "obj");
    LispObject_ptr obj = arg->next(true);
    if (obj->get_type() != LISPOBJECT_ATOM)
      return make_ptr(LispObject(false));

    LispAtom_ptr val = obj->get_value_atom();
    return make_ptr(LispObject(val->get_type() == LISPATOM_STRING));
  }
};
