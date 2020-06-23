#pragma once

#include <map>
#include <string>

#include "../types.hpp"
#include "../object/object.hpp"
#include "env_types.hpp"

class LispEnvironment {
  private:
    std::map<std::string, LispObject_ptr> objects_map;
    std::map<std::string, LispBuiltin_ptr> builtin_functions_map;
    std::map<std::string, LispFunction_ptr> lisp_functions_map;
    bool readonly = false;
    LispEnvironment_ptr parent;

  public:
    LispEnvironment();
    LispEnvironment(LispEnvironment_ptr parent, bool readonly = false);

    static std::string repr_type(LispEnvironment_Type type) {

      switch (type) {
        case LISPENV_OBJ:
          return "object";
        case LISPENV_LFUNC:
        case LISPENV_BFUNC:
          return "function";
        case LISPENV_NOTFOUND:
          return "absent";
      }

      throw AuthorError("EnvType not handled in LispEnvironment::repr_type");
    }

    void add(std::string name, LispObject_ptr obj);
    void add(std::string name, LispBuiltin_ptr val);
    void add(std::string name, LispFunction_ptr val);
    void add_something(std::string name, LispObject_ptr obj, LispBuiltin_ptr bfunc, LispFunction_ptr lfunc);

    LispEnvironment_Type get(LispSymbol_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);
    LispEnvironment_Type get(LispObject_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);
    LispEnvironment_Type get(std::string name, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);
    LispEnvironment_ptr get_parent() const {
      return this->parent;
    }

};
