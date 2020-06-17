#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"


// Convert object to string.
std::string LispObject::str()
{

  switch (this->type) {
    case LISPOBJECT_ATOM:
      return this->value_atom->str();

    case LISPOBJECT_LIST:
      return this->value_list->str();

    case LISPOBJECT_SYMBOL:
      return this->value_symbol->get_name();
  }

  throw AuthorError("Unknown type encountered in LispObject::str()!");
}




// Gets a string representation of the object's value.
std::string LispObject::repr() const
{

  switch (this->type) {
    case LISPOBJECT_ATOM:
      return this->value_atom->repr();

    case LISPOBJECT_LIST:
      return this->value_list->repr();

    case LISPOBJECT_SYMBOL:
      return this->value_symbol->get_name();
  }

  throw AuthorError("Unknown type encountered in LispObject::repr()!");
}




// Gets a string representation of the object's type (Atom, Symbol or List)
std::string LispObject::repr_type()
{
  return this->repr_type(this->type);
}

std::string LispObject::repr_type(LispObject_Type type)
{

  switch (type) {
    case LISPOBJECT_ATOM:
      return "Atom";

    case LISPOBJECT_LIST:
      return "List";

    case LISPOBJECT_SYMBOL:
      return "Symbol";
  }

  throw AuthorError("Unknown type encountered in LispObject::repr_type()!");
}




//
bool LispObject::is_truthy(LispEnvironment_ptr env)
{
  switch (this->type) {

    case LISPOBJECT_ATOM:
      return this->value_atom->is_truthy();

    case LISPOBJECT_LIST:
      return this->value_list->count();

    case LISPOBJECT_SYMBOL:
      if (env)
        return this->eval(env)->is_truthy();
      else
        throw TypeError("Truthy-ness of a symbol is ambiguous.");
  }

  throw AuthorError("Unknown type encountered in LispObject::is_truthy()!");
}

LispObject::LispObject() {
  this->type = LISPOBJECT_LIST;
  this->value_list = make_ptr(LispList());
}
