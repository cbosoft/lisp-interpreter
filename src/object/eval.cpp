#include "../util/debug.hpp"
#include "../env/env.hpp"
#include "../symbol/symbol.hpp"
#include "../list/list.hpp"
#include "../builtins/builtin.hpp"
#include "../function/function.hpp"
#include "singletons.hpp"
#include "object.hpp"

LispObject_ptr LispObject::eval(LispEnvironment_ptr env)
{
  inc_tab();
  try {
    auto rv = this->_eval(env);
    dec_tab();
    return rv;
  }
  catch (const Exception &e) {
    debug_message(Formatter() << "extra info " << this->str());
    throw Exception(e, this->repr_source());
  }
}

LispObject_ptr LispObject::_eval(LispEnvironment_ptr env)
{
  debug_message(Formatter() << "in eval");

  std::string fname;
  LispObject_ptr fn, var_obj;
  LispList_ptr list_args, list_obj;
  LispBuiltin_ptr var_bfunc;
  LispFunction_ptr var_lfunc;
  std::list<LispObject_ptr>::const_iterator list_elem, list_iter;
  LispEnvironment_Type envtype;
  int count = -1;

  if (env == NULL) throw AuthorError(Formatter() << "Eval called without environment.");

  debug_message(Formatter() << "eval " << this->repr_type());
  switch (this->type) {

  case LISPOBJECT_SYMBOL:
    if (env->get(this->value_symbol, &var_obj, &var_bfunc, &var_lfunc) != LISPENV_OBJ)
      throw NameError(Formatter() << "Symbol " << this->value_symbol->get_name() << " has no value as variable.");
    while (var_obj->get_type() == LISPOBJECT_SYMBOL) {
      if (env->get(var_obj->get_value_symbol()->get_name(), &var_obj, nullptr, nullptr) != LISPENV_OBJ) {
        throw NameError(Formatter() << "Symbol " << this->value_symbol->get_name() << " has no value as variable.");
      }
    }
    return var_obj;

  case LISPOBJECT_LIST:
    list_obj = this->value_list;
    
    count = list_obj->count();
    debug_message(Formatter() << "list has size " << count);
    if (count == 0)
      return nil;

    //debug_message(Formatter() << "list child is " << list_obj->first()->repr_type()); // invalid: child is eval'd before checking

    fn = list_obj->first();

    // if (fn->get_type() == LISPOBJECT_LIST) {
    //   list_iter = list_obj->begin();
    //   debug_message(Formatter() << "eval items in list, discard result");
    //   for (; list_iter != --list_obj->end(); ++list_iter) {
    //     debug_message(Formatter() << " ITEM TYPE: " << (*list_iter)->repr_type());
    //     (*list_iter)->eval(env);
    //   }
    //   debug_message(Formatter() << "eval final item in List");
    //   return (*(list_iter))->eval(env);
    // }
    // otherwise, list is not list of lists, is function call


    if (fn->get_type() != LISPOBJECT_SYMBOL) {
      debug_message(Formatter() << "evaluating name.");
      fn = fn->eval(env);
      debug_message(Formatter() << "AFTER");
    }

    if (fn->get_type() != LISPOBJECT_SYMBOL) throw SyntaxError(Formatter() << "List called as a function must start with a Symbol.");
    fname = fn->get_value_symbol()->get_name();

    debug_message(Formatter() << "applying function \"" << fname << "\".");

    envtype = env->get(fn, &var_obj, &var_bfunc, &var_lfunc);
    if (envtype == LISPENV_NOTFOUND)
      throw NameError(Formatter() << "Object \"" << fname << "\" not found in environment.");

    while ((envtype == LISPENV_OBJ) && (var_obj->get_type() == LISPOBJECT_SYMBOL)) {
      envtype = env->get(var_obj->get_value_symbol()->get_name(), &var_obj, &var_bfunc, &var_lfunc);
    }

    if (var_lfunc != NULL) {
      debug_message(Formatter() << "symbol " << fn->value_symbol << " is lisp function");
      list_args = list_obj->rest();
      return var_lfunc->eval(list_args, env);
    }
    else if (var_bfunc != NULL) {

      if (var_bfunc->is_macro()) {
        debug_message(Formatter() << "symbol " << fn->value_symbol << " is builtin (macro)");

        list_args = list_obj->rest();
      }
      else {
        debug_message(Formatter() << "symbol " << fn->value_symbol << " is builtin");

        list_args = std::make_shared<LispList>(LispList());
        for (list_iter = ++list_obj->begin(); list_iter != list_obj->end(); ++list_iter) {
          list_args->append((*list_iter)->eval(env));
        }
      }

      return var_bfunc->eval(list_args, env);
    }
    else {
      throw NameError(Formatter() << "Object \"" << fname << "\" has no value as function.");
    }

    break;

  default:
    break;
  }

  // default: root is atom (float, int, string, bool...)
  return std::make_shared<LispObject>(LispObject(*this)); // should return self? Or copy of self?
}
