#include <exception>

//#include "eval.h"
#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"


extern LispObject nil;


LispObject *LispObject::eval(LispEnvironment *env)
{
  debug_message(Formatter() << "in eval");

  std::string fname;
  LispObject *fn = NULL, *var_obj = NULL;
  LispList *list_args = NULL, *list_obj = NULL;
  LispBuiltin *var_bfunc = NULL;
  LispFunction *var_lfunc = NULL;
  std::list<LispObject *>::iterator list_elem, list_iter;


  if (env == NULL) throw std::runtime_error(Formatter() << "Eval called without environment.");

  debug_message(Formatter() << "eval " << this->repr_type());
  switch (this->type) {

  case LISPOBJECT_SYMBOL:
    var_obj = env->get_object(this->value_symbol->get_name());
    if (var_obj == NULL) throw std::runtime_error(Formatter() << "Symbol " << this->value_symbol->get_name() << " not defined.");
    return var_obj;

  case LISPOBJECT_LIST:
    list_obj = this->value_list;
    
    debug_message(Formatter() << "list has size " << list_obj->count());
    if (list_obj->count() == 0)
      return &nil;

    debug_message(Formatter() << "list child is " << list_obj->first()->repr_type());

    fn = list_obj->first();

    // if (fn->get_type() == LISPOBJECT_LIST) {
    //   list_iter = list_obj->begin();
    //   for (; list_iter != --list_obj->end(); ++list_iter) {
    //     debug_message(Formatter() << " ITEM TYPE: " << (*list_iter)->repr_type());
    //     (*list_iter)->eval(env);
    //   }
    //   return (*list_iter)->eval(env);
    // }
    // otherwise, list is not list of lists, is function call

    list_args = list_obj->rest();

    if (fn->get_type() != LISPOBJECT_SYMBOL) throw std::runtime_error(Formatter() << "List called as a function must start with a Symbol.");
    fname = fn->get_value_symbol()->get_name();

    if (env->get(fname, &var_obj, &var_bfunc, &var_lfunc) < 0) throw std::runtime_error(Formatter() << "Object \"" << fname << "\" not found in environment.");

    if (var_lfunc != NULL) {
      debug_message(Formatter() << "symbol " << fn->value_symbol << " is lisp function");
      return var_lfunc->eval(list_args, env);
    }
    else if (var_bfunc != NULL) {
      debug_message(Formatter() << "symbol " << fn->value_symbol << " is builtin");
      return var_bfunc->eval(list_args, env);
      
    }
    else {
      throw std::runtime_error(Formatter() << "Object \"" << fname << "\" has no value as funciton.");
    }
    
    break;
  }
  
  // default: root is atom (float, int, string, bool...)
  return this; // should return self? Or copy of self?
}




// 
LispObject  *LispFunction::eval(LispList *arg, LispEnvironment *env)
{
  unsigned long n_args_supplied = arg->count();
  if (n_args_supplied != this->arg_names.size()) throw "argument count doesn't match requirement";

  LispEnvironment *subenv = new LispEnvironment(env);
  auto arg_iter = arg->begin();
  auto argname_iter = this->arg_names.begin();
  for (; arg_iter != arg->end(); ++arg_iter, ++argname_iter)
    subenv->add((*argname_iter), (*arg_iter));

  return this->body->eval(subenv);
}




//
LispObject *LispList::eval_each(LispEnvironment *env) {
  debug_message("eval each");

  auto iter = this->begin();
  for (; iter != this->end(); ++iter)
    (*iter)->eval(env);

  return (*(++iter))->eval(env);
}
