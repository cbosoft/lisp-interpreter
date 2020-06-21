#pragma once
#include "../types.hpp"
#include "../util/formatter.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "../parser/parser.hpp"
#include "../symbol/symbol.hpp"
#include "builtin.hpp"


extern LispParser parser; // Boo: global var


class LispFunc_import : public virtual LispBuiltin {
  private:
    inline static const std::string name = "import";
    inline static const std::string doc = 
      "(import path)";

  public:
    LispFunc_import()
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
      int obj_type = path_obj->get_type();
      LispAtom_ptr path_atom;
      LispSymbol_ptr path_symb;
      std::string path;

      switch (obj_type) {

        case LISPOBJECT_ATOM:
          path_atom = path_obj->get_value_atom();
          if (path_atom->get_type() != LISPATOM_STRING) 
            throw ArgumentError(Formatter() << "In " << this->repr() << ": Argument \"path\" should be String. Got " << path_atom->repr_type() << ".");
          path = path_atom->get_value_string();
          break;

        case LISPOBJECT_SYMBOL:
          path_symb = path_obj->get_value_symbol();
          path = path_symb->get_name();
          break;

        case LISPOBJECT_LIST:
          throw ArgumentError(Formatter() << "In " << this->repr() << ": Argument \"path\" should be Atomistic (String) or Symbol. Got " << path_obj->repr_type() << ".");

        default:
          throw AuthorError("Unexpected type encountered in import.");
      }

      debug_message(Formatter() << "searching for module \"" << path << "\"");
      LispList_ptr root = parser.parse_module(path);
      return root->eval_each(env);
    }
};
