#pragma once
#include <map>
#include <thread>

#include "../types.hpp"
#include "../singletons.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"



static std::map<int, std::thread> threads_map;



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

      int id = threads_map.size();
      switch (type) {
        case LISPENV_LFUNC:
          debug_message("running lfunc in other thread");
          threads_map[id] = std::thread(&LispFunction::eval, lfunc, arg->rest(1), env);
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

      std::thread t;
      auto it = threads_map.find(id);

      if (it == threads_map.end())
        throw EnvironmentError(Formatter() << "thread with ID " << id << "not found.");

      t.swap(it->second);
      threads_map.erase(it);
      t.join();

      return std::make_shared<LispObject>(true);
    }
};
