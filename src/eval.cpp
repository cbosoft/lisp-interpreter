#include <exception>

//#include "eval.h"
#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"


extern LispObject nil;


LispObject *LispObject::eval(LispEnvironment *env)
{
  debug_message("IN EVAL\n");

  std::string fname;
  LispObject *fn = NULL, *var_obj = NULL;
  LispListElement *list_elem = NULL, *list_iter = NULL, *list_args;
  LispBuiltin *var_bfunc = NULL;
  LispFunction *var_lfunc = NULL;


  if (env == NULL) throw std::runtime_error(Formatter() << "eval called without environment.");

  debug_message("EVAL %s\n", this->repr_type().c_str());
  switch (this->type) {

  case LISPOBJECT_SYMBOL:
    var_obj = env->get_object(this->value_symbol->get_name());
    if (var_obj == NULL) throw std::runtime_error(Formatter() << "Symbol " << this->value_symbol->get_name() << " not defined.");
    return var_obj;

  case LISPOBJECT_LIST:
    list_elem = this->value_list;
    
    if (list_elem->get_value() == NULL) {
      return &nil;
    }

    debug_message("LIST CHILD IS %s\n", list_elem->get_value()->repr_type().c_str());

    fn = list_elem->get_value();

    if (fn->get_type() == LISPOBJECT_LIST) {

      list_iter = this->value_list;
      if (list_iter->get_next()) {

        debug_message("EVALUATE LIST CONTENTS IN TURN\n");
        while (list_iter->get_next()->get_value() != NULL) {
          debug_message(" ITEM TYPE: %s\n", list_iter->get_value()->repr_type());
          list_iter->get_value()->eval(env);
          list_iter = list_iter->get_next();
        }

      }
      return list_iter->get_value()->eval(env);
    }
    // otherwise, list is not list of lists, is function call
	
    list_args = list_elem->get_next();

    if (fn->get_type() != LISPOBJECT_SYMBOL) throw std::runtime_error(Formatter() << "List called as a function must start with a Symbol.");
    fname = fn->get_value_symbol()->get_name();

    if (env->get(fname, &var_obj, &var_bfunc, &var_lfunc) < 0) throw std::runtime_error(Formatter() << "Object \"" << fname << "\" not found in environment.");

    if (var_lfunc != NULL) {
      debug_message("SYMBOL %s IS LISP FUNCTION\n", fn->value_symbol);
      return var_lfunc->eval(list_args, env);
    }
    else if (var_bfunc != NULL) {
      debug_message("SYMBOL %s IS BUILTIN\n", fn->value_symbol);
      return var_bfunc->eval(list_args, env);
      
    }
    else {
      throw "object not found";
    }
    
    break;
  }
  
  // default: root is atom (float, int, string, bool...)
  return this; // should return self? Or copy of self?
}




// 
LispObject  *LispFunction::eval(LispListElement *arg, LispEnvironment *env)
{
  unsigned long n_args_supplied = arg->count();
  if (n_args_supplied != this->arg_names.size()) throw "argument count doesn't match requirement";

  LispEnvironment *subenv = new LispEnvironment(env);
  LispListElement *arg_iter = arg;
  for (auto arg_name : this->arg_names) {
    subenv->add_variable(arg_name, arg_iter->get_value());
  }

  return this->body->eval(subenv);
}
