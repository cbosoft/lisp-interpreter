#include "types.hpp"
#include "debug.hpp"
#include "formatter.hpp"


// Gets a string representation of the object's value.
std::string LispObject::repr()
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

  switch (this->type) {
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
bool LispObject::is_truthy()
{
  switch (this->type) {

    case LISPOBJECT_ATOM:
      return this->value_atom->is_truthy();

    case LISPOBJECT_LIST:
      return this->value_list->count();

    case LISPOBJECT_SYMBOL:
      throw TypeError("Truthy-ness of a symbol is ambiguous.");
  }

  throw AuthorError("Unknown type encountered in LispObject::is_truthy()!");
}
