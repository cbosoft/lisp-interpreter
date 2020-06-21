#include <exception>

//#include "eval.h"
#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "singletons.hpp"




LispObject_ptr LispObject::eval(LispEnvironment_ptr env)
{
  debug_message(Formatter() << "in eval");

  std::string fname;
  LispObject_ptr fn, var_obj;
  LispList_ptr list_args, list_obj;
  LispBuiltin_ptr var_bfunc;
  LispFunction_ptr var_lfunc;
  std::list<LispObject_ptr>::const_iterator list_elem, list_iter;
  int count = -1;

  if (env == NULL) throw AuthorError(Formatter() << "Eval called without environment.");

  debug_message(Formatter() << "eval " << this->repr_type());
  switch (this->type) {

  case LISPOBJECT_SYMBOL:
    if (env->get(this->value_symbol, &var_obj, &var_bfunc, &var_lfunc) != LISPENV_OBJ)
      throw NameError(Formatter() << "Symbol " << this->value_symbol->get_name() << " has no value as variable.");
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

    if (env->get(fn, &var_obj, &var_bfunc, &var_lfunc) == LISPENV_NOTFOUND)
      throw NameError(Formatter() << "Object \"" << fname << "\" not found in environment.");

    if (var_lfunc != NULL) {
      debug_message(Formatter() << "symbol " << fn->value_symbol << " is lisp function");

      if (var_lfunc->is_macro()) {
        list_args = list_obj->rest();
      }
      else {
        list_args = std::make_shared<LispList>(LispList());
        for (list_iter = ++list_obj->begin(); list_iter != list_obj->end(); ++list_iter) {
          list_args->append((*list_iter)->eval(env));
        }
      }

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




// 
LispObject_ptr LispFunction::eval(LispList_ptr arg, LispEnvironment_ptr env) const
{
  int n_args_supplied = arg->count();
  int n_args_max = this->arg_names.size();
  int n_args_min = n_args_max - this->arg_default_values.size();

  if ((n_args_supplied < n_args_min) or (n_args_supplied > n_args_max))
    throw ArgumentError(Formatter() 
        << "In " << this->name << ": argument count doesn't match requirement. Got " << n_args_supplied << " expected between " << n_args_min << " and " << n_args_max << ".");

  debug_message(Formatter() << "lisp function called with " << n_args_supplied << " args.");

  LispEnvironment_ptr subenv = std::make_shared<LispEnvironment>(env);

  for (size_t dv_i = 0; dv_i < this->arg_default_values.size(); dv_i++) {
    int an_i = n_args_min + dv_i;
    subenv->add(this->arg_names[an_i], this->arg_default_values[dv_i]);
    debug_message(Formatter() << "add default val to subenv " << this->arg_names[an_i] << " = " << this->arg_default_values[dv_i]->str());
  }

  auto arg_iter = arg->begin();
  auto argname_iter = this->arg_names.begin();
  for (; arg_iter != arg->end(); ++arg_iter, ++argname_iter) {
    subenv->add((*argname_iter), (*arg_iter));
  }

  this->result = this->body->eval_each(subenv);
  return this->result;
}
