#include "types.hpp"
#include "exception_check.hpp"
#include "formatter.hpp"
#include "debug.hpp"

void LispEnvironment::add_something(std::string name, LispObject_ptr obj, LispBuiltin_ptr bfunc, LispFunction_ptr lfunc)
{
  if (this->readonly) {
    debug_message(Formatter() << "readonly environment " << this->readonly);
    if (this->parent == NULL) {
      throw EnvironmentError("Tried to add to read only environment with no parent.");
    }

    this->parent->add_something(name, obj, bfunc, lfunc);
    return;
  }

  if (obj != NULL) {
    debug_message(Formatter() << "adding var \"" << name << "\" to environment as variable");
    this->add(name, obj);
  }
  else if (bfunc != NULL) {
    debug_message(Formatter() << "adding var \"" << name << "\" to environment as function (builtin)");
    this->add(name, bfunc);
  }
  else if (lfunc != NULL) {
    debug_message(Formatter() << "adding var \"" << name << "\" to environment as function (lisp)");
    this->add(name, lfunc);
  }
  else {
    throw EnvironmentError("Tried to add nothing to environment.");
  }
}




void LispEnvironment::add(std::string name, LispObject_ptr obj) 
{ 
  if (this->readonly) {
    if (this->parent == NULL)
      throw EnvironmentError("Tried to add to read only environment with no parent.");

    this->parent->add(name, obj);
    return;
  }
  this->objects_map.insert_or_assign(name, obj); 
}

void LispEnvironment::add(std::string name, LispBuiltin_ptr val)
{ 
  if (this->readonly) {
    if (this->parent == NULL)
      throw EnvironmentError("Tried to add to read only environment with no parent.");

    this->parent->add(name, val);
    return;
  }
  this->builtin_functions_map.insert_or_assign(name, val); 
}

void LispEnvironment::add(std::string name, LispFunction_ptr val)
{ 
  if (this->readonly) {
    if (this->parent == NULL)
      throw EnvironmentError("Tried to add to read only environment with no parent.");

    this->parent->add(name, val);
    return;
  }
  this->lisp_functions_map.insert_or_assign(name, val); 
}




extern struct environment_table_row builtins[];
LispEnvironment::LispEnvironment()
{
  this->parent = NULL;
  int i = 0;
  struct environment_table_row row_i = builtins[0];
  for (i = 0, row_i = builtins[i]; row_i.name != NULL; i++, row_i = builtins[i]) {
    debug_message(Formatter() << row_i.name << row_i.obj << row_i.bfunc << row_i.lfunc);
    this->add_something(row_i.name, row_i.obj, row_i.bfunc, row_i.lfunc);
    if (row_i.alias != NULL) this->add_something(row_i.alias, row_i.obj, row_i.bfunc, row_i.lfunc);
  }
  this->readonly = false;
}


LispEnvironment_Type LispEnvironment::get(LispObject_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf)
{
  type_check_one(sym, LISPOBJECT_SYMBOL, "get", "symbol");

  return this->get(sym->get_value_symbol(), obj, bf, lf);
}

LispEnvironment_Type LispEnvironment::get(LispSymbol_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf)
{
  auto name = sym->get_name();

  auto type = this->get(name, obj, bf, lf);
  LispEnvironment_ptr par = this->get_parent();

  if (type == LISPENV_OBJ) {
    if ((*obj)->get_type() == LISPOBJECT_SYMBOL) {
      if ((*obj)->get_value_symbol()->get_name() == name) {

        debug_message("object resolved to itself");
        if (par) {
          debug_message("  --> trying parent");
          return par->get(sym, obj, bf, lf);
        }
        else {
          (*obj) = nullptr;
          return LISPENV_NOTFOUND;
        }

      }
    }
  }

  return type;
}

LispEnvironment_Type LispEnvironment::get(std::string name, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf)
{
  debug_message(Formatter() << "looking for \"" << name << "\" in environment");
  if (obj) (*obj) = NULL;
  if (bf) (*bf) = NULL;
  if (lf) (*lf) = NULL;

  auto obj_iter = this->objects_map.find(name);
  if (obj_iter != this->objects_map.end()) {
    debug_message(Formatter() << "found \"" << name << "\" in environment as object " << obj_iter->second->str());
    if (obj) (*obj) = obj_iter->second;
    return LISPENV_OBJ;
  }

  auto bf_iter = this->builtin_functions_map.find(name);
  if (bf_iter != this->builtin_functions_map.end()) {
    debug_message(Formatter() << "found \"" << name << "\" in environment as builtin function");
    if (bf) (*bf) = bf_iter->second;
    return LISPENV_BFUNC;
  }

  auto lf_iter = this->lisp_functions_map.find(name);
  if (lf_iter != this->lisp_functions_map.end()) {
    debug_message(Formatter() << "found \"" << name << "\" in environment as lisp function");
    if (lf) (*lf) = lf_iter->second;
    return LISPENV_LFUNC;
  }

  if (this->parent == NULL) {
    debug_message(Formatter() << "object not found, and no parent. get failed.");
    return LISPENV_NOTFOUND;
  }

  return this->parent->get(name, obj, bf, lf);
}
