#include <sstream>
#include "atom.hpp"




std::string LispAtom::repr()
{
  std::stringstream ss;
  switch (this->type()) {

    case LISPATOM_INT:
      ss << this->value_int;
      break;

    case LISPATOM_FLOAT:
      ss << this->value_float;
      break;

    case LISPATOM_STRING:
      ss << this->value_string;
      break;

  }
  return ss.str();
}




//
long LispAtom::cast_to_int()
{
  switch (this->type()) {

    case LISPATOM_INT:
      return this->value_int;

    case LISPATOM_FLOAT:
      return (long)this->value_float;

    case LISPATOM_STRING:
      std::stringstream ss(this->value_string);
      long rv;
      ss >> rv;
      return rv;

  }

  throw "Unknown type encountered in cast";
}
