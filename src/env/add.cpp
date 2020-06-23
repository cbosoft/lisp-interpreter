#include "env.hpp"


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
