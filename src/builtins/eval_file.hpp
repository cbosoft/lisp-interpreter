#pragma once
#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"

extern LispParser parser; // Boo: global var

class LispFunc_eval_file : public virtual LispBuiltin {
  private:
    inline static const std::string name = "eval-file";
    inline static const std::string doc = 
      "(eval-file path)";

  public:
    LispFunc_eval_file()
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

      narg_check(arg, 1, this->repr(), "path");

      LispObject_ptr path_obj = arg->next(true);
      type_check_atom(path_obj, LISPATOM_STRING, this->repr(), "path");
      LispAtom_ptr path_atom = path_obj->get_value_atom();

      LispList_ptr root = parser.parse_file(path_atom->get_value_string());
      return root->eval_each(env);
    }
};
