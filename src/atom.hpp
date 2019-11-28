#pragma once
#include <string>

#define OP(O) LispAtom operator O(LispAtom obj) { \
\
  switch (this->get_type()) {\
    case LISPATOM_INT:\
      return LispAtom((int)(this->value_int O obj.cast_to_int() ));\
    case LISPATOM_FLOAT:\
      return LispAtom((double)(this->value_float O obj.cast_to_float() ));\
    case LISPATOM_STRING:\
      throw "Numerical operation O not defined for string.";\
  }\
}

enum LISPATOM_TYPE{
  LISPATOM_INT,
  LISPATOM_FLOAT,
  LISPATOM_STRING
};

class LispAtom {
  private:
    long value_int;
    double value_float;
    std::string value_string;
    int get_type();

  public:
    LispAtom(int value) { this->value_int = value; }
    LispAtom(double value) { this->value_float = value; }
    LispAtom(std::string value) { this->value_string = value; }

    int type();
    std::string repr();

    LispAtom operator +(LispAtom obj) {

      switch (this->get_type()) {

        case LISPATOM_INT:
          return LispAtom((int)(this->value_int + obj.cast_to_int() ));

        case LISPATOM_FLOAT:
          return LispAtom((double)(this->value_float + obj.cast_to_float() ));

        case LISPATOM_STRING:
          return LispAtom(this->value_string + obj.cast_to_string() );
      }
    }
   
   
    OP(-); OP(*); OP(/);
    OP(>); OP(>=); OP(<); OP(<=); OP(==);

    long cast_to_int();
    double cast_to_float();
    std::string cast_to_string();
};
