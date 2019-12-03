#pragma once
#include <exception>
#include <string>
#include <iostream>

#include "colour.hpp"




class Exception : public std::exception {
  private:
    std::string detail;
    std::string type = "";
  public:
    Exception(std::string detail, std::string type) { this->detail = detail; this->type = type; }

    void pretty_print() {
      std::cerr << BOLD << BG_RED << this->type << RESET << ": " << this->detail << std::endl;
    }
};





#define EXCEPTION(N,S)\
  class N : virtual public Exception {\
    public:\
           N(std::string detail) : Exception(detail, S) { }\
  };

class ArgumentError : virtual public Exception {
  public:
    ArgumentError(std::string detail) : Exception(detail, "ArgumentError") {  }
};

class NameError : virtual public Exception {
  public:
    NameError(std::string detail) : Exception(detail, "NameError") {  }
};

EXCEPTION(TypeError, "TypeError");
EXCEPTION(SyntaxError, "SyntaxError");
EXCEPTION(NotImplementedError, "NotImplementedError");
EXCEPTION(DivideByZeroError, "DivideByZeroError");
