#include <exception>

//#include "eval.h"
#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"
#include "singletons.hpp"




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
