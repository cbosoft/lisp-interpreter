#pragma once
#include <map>
#include <thread>

#include "../types.hpp"
#include "../object/singletons.hpp"
#include "../util/exception.hpp"
#include "../util/exception_check.hpp"
#include "../env/env.hpp"
#include "../function/function.hpp"
#include "../symbol/symbol.hpp"
#include "../atom/atom.hpp"
#include "../object/object.hpp"
#include "../list/list.hpp"
#include "builtin.hpp"



static std::map<int, std::thread> threads_map;
static std::map<int, LispFunction_ptr> threaded_funcs_map;



class LispFunc_thread : public virtual LispBuiltin {
  private:
    inline static const std::string name = "thread";
    inline static const std::string doc =
      "(thread func)";

  public:
    LispFunc_thread()
    {
    }

    const std::string repr() const
    {
      return "Func(thread,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      narg_check_min(arg, 1, this->repr(), "func");

      LispObject_ptr func_name = arg->next(true);
      type_check_one(func_name, LISPOBJECT_SYMBOL, this->repr(), "func");

      LispFunction_ptr lfunc;
      LispBuiltin_ptr bfunc;
      std::string symname = func_name->get_value_symbol()->get_name();
      auto type = env->get(symname, nullptr, &bfunc, &lfunc);

      static int id = -1;
      id ++;
      switch (type) {
        case LISPENV_LFUNC:
          debug_message("running lfunc in other thread");
          threads_map[id] = std::thread(&LispFunction::eval, lfunc, arg->rest(1), env);
          threaded_funcs_map[id] = lfunc;
          break;

        case LISPENV_BFUNC:
          //std::thread(&LispBuiltin::eval, bfunc, env);
          //break;

        case LISPENV_OBJ:
          throw TypeError(Formatter() << "In `thread`: symbol \"" << symname << "\" must be func, not variable.");

        case LISPENV_NOTFOUND:
          throw TypeError(Formatter() << "In `thread`: symbol \"" << symname << "\" not found in environment.");
      }

      return std::make_shared<LispObject>(id);
    }
};


class LispFunc_join : public virtual LispBuiltin {
  private:
    inline static const std::string name = "join";
    inline static const std::string doc =
      "(join id)";

  public:
    LispFunc_join()
    {
    }

    const std::string repr() const
    {
      return "Func(join,builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;

      narg_check(arg, 1, this->repr(), "id");

      LispObject_ptr id_obj = arg->next(true);
      type_check_atom(id_obj, LISPATOM_INT, this->repr(), "id");

      int id = id_obj->get_value_atom()->get_value_int();

      {
        std::thread t;
        auto it = threads_map.find(id);

        if (it == threads_map.end())
          throw EnvironmentError(Formatter() << "thread with ID " << id << "not found.");

        t.swap(it->second);
        threads_map.erase(it);
        t.join();
      }

      LispObject_ptr result;
      {
        auto it = threaded_funcs_map.find(id);

        if (it == threaded_funcs_map.end())
          throw EnvironmentError(Formatter() << "thread with ID " << id << "not found.");

        result = it->second->get_result();
        threaded_funcs_map.erase(it);
      }

      return result;
    }
};
