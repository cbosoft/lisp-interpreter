#include "types.hpp"
#include "formatter.hpp"
#include "debug.hpp"

void LispEnvironment::add_something(std::string name, LispObject_ptr obj, LispBuiltin_ptr bfunc, LispFunction_ptr lfunc)
{
  if (obj != NULL) {
    debug_message(Formatter() << "adding var \"" << name << "\" to environment as variable");
    this->add(name, obj);
  }
  else if (bfunc != NULL) {
    debug_message(Formatter() << "adding var \"" << name << "\" to environment as function (builtin)");
    this->add(name, bfunc);
  }
  else {
    debug_message(Formatter() << "adding var \"" << name << "\" to environment as function (lisp)");
    this->add(name, lfunc);
  }
}




void LispEnvironment::add(std::string name, LispObject_ptr obj) 
{ 
  this->objects_map.insert_or_assign(name, obj); 
}

void LispEnvironment::add(std::string name, LispBuiltin_ptr val)
{ 
  this->builtin_functions_map.insert_or_assign(name, val); 
}

void LispEnvironment::add(std::string name, LispFunction_ptr val)
{ 
  this->lisp_functions_map.insert_or_assign(name, val); 
}




extern struct environment_table_row builtin_functions[];
LispEnvironment::LispEnvironment()
{
  this->parent = NULL;
  int i = 0;
  struct environment_table_row row_i = builtin_functions[0];
  for (i = 0, row_i = builtin_functions[i]; row_i.name != NULL; i++, row_i = builtin_functions[i]) {
    this->add_something(row_i.name, row_i.obj, row_i.bfunc, row_i.lfunc);
    if (row_i.alias != NULL) this->add_something(row_i.alias, row_i.obj, row_i.bfunc, row_i.lfunc);
  }
}




int LispEnvironment::get(std::string name, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf)
{
  debug_message(Formatter() << "looking for \"" << name << "\" in environment");
  (*obj) = NULL;
  (*bf) = NULL;
  (*lf) = NULL;

  auto obj_iter = this->objects_map.find(name);
  if (obj_iter != this->objects_map.end()) {
    (*obj) = obj_iter->second;
    return LISPENV_OBJ;
  }

  auto bf_iter = this->builtin_functions_map.find(name);
  if (bf_iter != this->builtin_functions_map.end()) {
    (*bf) = bf_iter->second;
    return LISPENV_BFUNC;
  }

  auto lf_iter = this->lisp_functions_map.find(name);
  if (lf_iter != this->lisp_functions_map.end()) {
    (*lf) = lf_iter->second;
    return LISPENV_LFUNC;
  }

  if (this->parent == NULL) {
    debug_message(Formatter() << "object not found, and no parent. get failed.");
    return -1;
  }

  return this->parent->get(name, obj, bf, lf);
}
