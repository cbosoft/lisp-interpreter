#pragma once

#include <memory>
#include <string>

#include "../printable/printable.hpp"
#include "atom_types.hpp"

class LispAtom;
typedef std::shared_ptr<LispAtom> LispAtom_ptr;

class LispAtom : virtual public Printable {
  private:
    long value_int;
    double value_float;
    std::string value_string;
    LispAtom_Type type;

  public:
    LispAtom(long value) {
      this->type = LISPATOM_INT;
      this->value_int = value; 
    }
    LispAtom(bool value) : LispAtom((long)value) {};
    LispAtom(int value) : LispAtom((long)value) {};

    LispAtom(double value) { 
      this->type = LISPATOM_FLOAT;
      this->value_float = value; 
    }
    LispAtom(float value) : LispAtom((double)value) {};

    LispAtom(std::string value);
    LispAtom(const char* value) : LispAtom(std::string(value)) {};

    LispAtom_Type get_type() { return this->type; }
    const std::string str() const;
    const std::string repr() const;
    std::string repr_type();
    static std::string repr_type(LispAtom_Type& type);
    long get_value_int(){ return this->value_int; }
    double get_value_float(){ return this->value_float; }
    std::string get_value_string(){ return this->value_string; }

    bool is_truthy();

    LispAtom_ptr add(LispAtom_ptr obj);
    LispAtom_ptr subtract(LispAtom_ptr obj);
    LispAtom_ptr multiply(LispAtom_ptr obj);
    LispAtom_ptr divide(LispAtom_ptr obj);
    LispAtom_ptr power(LispAtom_ptr obj);
    LispAtom_ptr modulo(LispAtom_ptr obj);

    LispAtom_ptr gt(LispAtom_ptr obj);
    LispAtom_ptr ge(LispAtom_ptr obj);
    LispAtom_ptr lt(LispAtom_ptr obj);
    LispAtom_ptr le(LispAtom_ptr obj);
    LispAtom_ptr eq(LispAtom_ptr obj);

    long cast_to_int();
    double cast_to_float();
    std::string cast_to_string();
};
