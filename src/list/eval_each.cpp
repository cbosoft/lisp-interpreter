#include "../object/object.hpp"
#include "list.hpp"

//
LispObject_ptr LispList::eval_each(LispEnvironment_ptr env) const
{
  debug_message("eval each");

  auto iter = this->begin();
  for (; iter != --this->end(); ++iter) {
    debug_message(Formatter() << "eval List items, discard result (" << (*iter) << ")");
    (*iter)->eval(env);
  }

  debug_message(Formatter() << "eval last List item, return result (" << (*iter) << ")");
  return (*(iter))->eval(env);
}
