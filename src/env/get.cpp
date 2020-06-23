#include "../types.hpp"
#include "../util/exception_check.hpp"
#include "../symbol/symbol.hpp"
#include "env.hpp"


LispEnvironment_Type LispEnvironment::get(LispObject_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf)
{
  type_check(sym, LISPOBJECT_SYMBOL, "get", "symbol");

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
