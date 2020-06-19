#pragma once
#include <string>
#include <sstream>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../singletons.hpp"



class LispFunc_catch : public virtual LispBuiltin {
  private:
    inline static const std::string name = "catch";
    inline static const std::string doc =
      "(catch exception callback &rest body)";

  public:
    LispFunc_catch()
    {
      this->set_macro();
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
      narg_check_min(arg, 3, this->name, "exception callback callback-args &rest body");

      auto exc_obj = arg->next(true);

      std::string exc_str;
      if (exc_obj->get_type() == LISPOBJECT_ATOM) {
        auto exc_atom = exc_obj->get_value_atom();
        if (exc_atom->get_type() == LISPATOM_STRING) {
          exc_str = exc_atom->get_value_string();
        }
      }

      auto cb_obj = arg->next();
      type_check_symbol(cb_obj, LISPENV_LFUNC, this->name, "callback", env);
      LispFunction_ptr cb;
      env->get(cb_obj, nullptr, nullptr, &cb);

      auto cb_args_obj = arg->next();
      type_check_one(cb_args_obj, LISPOBJECT_LIST, this->name, "callback-args");
      auto cb_args = cb_args_obj->get_value_list();

      auto body = arg->rest(3);

      LispObject_ptr rv;

      try {
        rv = body->eval_each(env);
      }
      catch (const Exception &e) {

        debug_message(Formatter() << "in catch: exception \"" << e.get_type() << "\" thrown, checkin if should raise");
        if (e.get_type().compare(exc_str) == 0) {
          return cb->eval(cb_args, env);
        }
        else {
          throw e;
        }
      }
      catch (const LispObject_ptr &o) {
        if (exc_obj->eq(o)) {
          return cb->eval(cb_args, env);
        }
        else {
          throw o;
        }
      }

      return rv;
    }
};

