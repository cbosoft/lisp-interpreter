#include <exception>

//#include "eval.h"
#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"


extern LispObject nil;


LispObject_ptr LispObject::eval(LispEnvironment_ptr env)
{
  debug_message(Formatter() << "in eval");

  std::string fname;
  LispObject_ptr fn, var_obj;
  LispList_ptr list_args, list_obj;
  LispBuiltin_ptr var_bfunc;
  LispFunction_ptr var_lfunc;
  std::list<LispObject_ptr>::iterator list_elem, list_iter;


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
    list_obj->print();
    if (list_obj->count() == 0)
      return std::make_shared<LispObject>(nil); // TODO use the same shared ptr for all nil?

    debug_message(Formatter() << "list child is " << list_obj->first()->repr_type());

    fn = list_obj->first();

    if (fn->get_type() == LISPOBJECT_LIST) {
      list_iter = list_obj->begin();
      debug_message(Formatter() << "eval items in list, discard result");
      for (; list_iter != --list_obj->end(); ++list_iter) {
        debug_message(Formatter() << " ITEM TYPE: " << (*list_iter)->repr_type());
        (*list_iter)->eval(env);
      }
      debug_message(Formatter() << "eval final item in List");
      return (*(list_iter))->eval(env);
    }
    // otherwise, list is not list of lists, is function call

    list_args = list_obj->rest();

    if (fn->get_type() != LISPOBJECT_SYMBOL) throw std::runtime_error(Formatter() << "List called as a function must start with a Symbol.");
    fname = fn->get_value_symbol()->get_name();

    debug_message(Formatter() << "applying function \"" << fname << "\".");

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
  return std::shared_ptr<LispObject>(this); // should return self? Or copy of self?
}




// 
LispObject_ptr LispFunction::eval(LispList_ptr arg, LispEnvironment_ptr env)
{
  unsigned long n_args_supplied = arg->count();
  if (n_args_supplied != this->arg_names.size()) throw "argument count doesn't match requirement";
  debug_message(Formatter() << "lisp function called with " << n_args_supplied << " args.");

  LispEnvironment_ptr subenv = std::make_shared<LispEnvironment>(LispEnvironment(env));
  auto arg_iter = arg->begin();
  auto argname_iter = this->arg_names.begin();
  for (; arg_iter != arg->end(); ++arg_iter, ++argname_iter)
    subenv->add((*argname_iter), (*arg_iter));

  return this->body->eval(subenv);
}




//
LispObject_ptr LispList::eval_each(LispEnvironment_ptr env) {
  debug_message("eval each");

  auto iter = this->begin();
  for (; iter != --this->end(); ++iter) {
    debug_message(Formatter() << "eval List items, discard result (" << (*iter) << ")");
    (*iter)->eval(env);
  }

  debug_message(Formatter() << "eval last List item, return result (" << (*iter) << ")");
  return (*(iter))->eval(env);
}
