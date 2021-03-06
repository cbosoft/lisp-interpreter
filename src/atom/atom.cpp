#include <sstream>

#include "../types.hpp"
#include "../util/exception.hpp"
#include "../util/formatter.hpp"
#include "atom.hpp"

LispAtom::LispAtom(std::string s)
{
  this->type = LISPATOM_STRING;

  std::stringstream ss;
  bool escape = false;

  for (char ch : s) {

    if (!escape) {
      if (ch == '\\') {
        escape = true;
      }
      else {
        ss << ch;
      }
    }
    else {
      switch (ch) {
        case 'n':
          ss << "\n";
          break;

        case 't':
          ss << "\t";
          break;

        case 'r':
          ss << "\r";
          break;

        case '\\':
          ss << "\\";
          break;

        default:
          ss << "\\" << ch;
          break;
      }
      escape = false;
    }

  }

  this->value_string = ss.str();
}




const std::string LispAtom::str() const
{
  std::stringstream ss;
  switch (this->type) {

    case LISPATOM_INT:
      ss << this->value_int;
      break;

    case LISPATOM_FLOAT:
      ss << this->value_float;
      break;

    case LISPATOM_STRING:
      ss << this->value_string;
      break;

    default:
      throw AuthorError("Unknown type encountered in LispAtom::str()!");

  }
  return ss.str();
}




const std::string LispAtom::repr() const
{
  std::stringstream ss;
  switch (this->type) {

    case LISPATOM_INT:
      ss << this->value_int;
      break;

    case LISPATOM_FLOAT:
      ss << this->value_float;
      break;

    case LISPATOM_STRING:
      ss << "\"" << this->value_string << "\"";
      break;

    default:
      throw AuthorError("Unknown type encountered in LispAtom::repr()!");

  }
  return ss.str();
}




std::string LispAtom::repr_type()
{
  return LispAtom::repr_type(this->type);
}




std::string LispAtom::repr_type(LispAtom_Type& type)
{
  switch (type) {

    case LISPATOM_INT:
      return "Atom(Int)";

    case LISPATOM_FLOAT:
      return "Atom(Float)";

    case LISPATOM_STRING:
        return "Atom(String)";

  }
  
  throw AuthorError("In LispAtom::repr_type(): Unknown type encountered!");
}




//
long LispAtom::cast_to_int()
{
  switch (this->type) {

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

  throw AuthorError("Unknown type encountered in LispAtom::cast_to_int()!");
}




//
double LispAtom::cast_to_float()
{
  switch (this->type) {

    case LISPATOM_INT:
      return (double)this->value_int;

    case LISPATOM_FLOAT:
      return this->value_float;

    case LISPATOM_STRING:
      std::stringstream ss(this->value_string);
      double rv;
      ss >> rv;
      return rv;

  }

  throw AuthorError("Unknown type encountered in cast");
}




//
std::string LispAtom::cast_to_string()
{

  std::stringstream ss;
  switch (this->type) {

    case LISPATOM_INT:
      ss << this->value_int;
      return ss.str();

    case LISPATOM_FLOAT:
      ss << this->value_int;
      return ss.str();

    case LISPATOM_STRING:
      return this->value_string;

  }

  throw AuthorError("Unknown type encountered in cast");
}




// Does the value represent boolean "true"?
bool LispAtom::is_truthy()
{

  std::stringstream ss;
  switch (this->type) {

    case LISPATOM_INT:
      return (bool)this->value_int;

    case LISPATOM_FLOAT:
      return (bool)this->value_float;

    case LISPATOM_STRING:
      return (bool)this->value_string.size();

  }

  throw AuthorError("Unknown type encountered in LispAtom::is_truthy()!");
}



// add obj to this and return
LispAtom_ptr LispAtom::add(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int + obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float + obj->cast_to_float()));

    case LISPATOM_STRING:
      return std::make_shared<LispAtom>(LispAtom(this->value_string + obj->cast_to_string()));
  }

  throw TypeError("In LispAtom::add(): Unknown type encountered!");
}




// subtract obj from this
LispAtom_ptr LispAtom::subtract(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int - obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float - obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::subtract(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::subtract(): Unknown type encountered!");
}




// multiply this by obj
LispAtom_ptr LispAtom::multiply(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int * obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float * obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::multiply(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::multiply(): Unknown type encountered!");
}




// divide this by obj
LispAtom_ptr LispAtom::divide(LispAtom_ptr obj) {

  long longval;
  double dblval;
  switch (this->get_type()) {

    case LISPATOM_INT:
      longval = obj->cast_to_int();
      if (longval == 0) throw DivideByZeroError("In LispAtom::divide(): right operand cannot be zero.");
      return std::make_shared<LispAtom>(LispAtom(this->value_int / longval));

    case LISPATOM_FLOAT:
      dblval = obj->cast_to_float();
      if (dblval == 0) throw DivideByZeroError("In LispAtom::divide(): right operand cannot be zero.");
      return std::make_shared<LispAtom>(LispAtom(this->value_float / obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::divide(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::divide(): Unknown type encountered!");
}




// modulo this by obj
LispAtom_ptr LispAtom::modulo(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int % obj->cast_to_int()));

    case LISPATOM_FLOAT:
    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::modulo(): Invalid type encountered! Got " << this->repr_type() << ", expected Int.");
  }

  throw TypeError("In LispAtom::modulo(): Unknown type encountered!");
}




// is this > obj?
LispAtom_ptr LispAtom::gt(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int > obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float > obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::gt(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::gt(): Unknown type encountered!");
}




// is this >= obj?
LispAtom_ptr LispAtom::ge(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int >= obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float >= obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::ge(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::ge(): Unknown type encountered!");
}




// is this < obj?
LispAtom_ptr LispAtom::lt(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int < obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float < obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::lt(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::lt(): Unknown type encountered!");
}




// is this <= obj?
LispAtom_ptr LispAtom::le(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int <= obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float <= obj->cast_to_float()));

    case LISPATOM_STRING:
      throw TypeError(Formatter() << "In LispAtom::le(): Invalid type encountered! Got " << this->repr_type() << ", expected Int or Float.");
  }

  throw TypeError("In LispAtom::le(): Unknown type encountered!");
}




// is this == obj?
LispAtom_ptr LispAtom::eq(LispAtom_ptr obj) {

  switch (this->get_type()) {

    case LISPATOM_INT:
      return std::make_shared<LispAtom>(LispAtom(this->value_int == obj->cast_to_int()));

    case LISPATOM_FLOAT:
      return std::make_shared<LispAtom>(LispAtom(this->value_float == obj->cast_to_float()));

    case LISPATOM_STRING:
      return std::make_shared<LispAtom>(LispAtom(this->value_string == obj->cast_to_string()));
  }

  throw TypeError("In LispAtom::eq(): Unknown type encountered!");
}
