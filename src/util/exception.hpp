#pragma once
#include <exception>
#include <string>
#include <iostream>

#include "colour.hpp"



class Exception : public std::exception {
  private:
    std::string detail = "";
    std::string type = "";
    std::string cause = "";

  public:
    Exception(std::string detail, std::string type, std::string cause) 
    { 
      this->detail = detail; 
      this->type = type; 
      this->cause = cause;
    }

    Exception(std::string detail, std::string type) 
    { 
      this->detail = detail; 
      this->type = type; 
    }

    Exception(const Exception &other, std::string cause)
      : detail(other.detail), type(other.type), cause(cause)
    {
      //
    }

    void pretty_print() const
    {
      std::cerr << BOLD << BG_RED << this->type << RESET << ": " << this->detail << std::endl;

      if (this->cause.size())
        std::cerr << this->cause << std::endl;
    }

    const std::string &get_type() const
    {
      return this->type;
    }
};





#define EXCEPTION(N,S)\
  class N : virtual public Exception {\
    public:\
           N(std::string detail, std::string cause) : Exception(detail, S, cause) { }\
           N(std::string detail) : Exception(detail, S) { }\
  };

EXCEPTION(ArgumentError, "ArgumentError");
EXCEPTION(NameError, "NameError");
EXCEPTION(TypeError, "TypeError");
EXCEPTION(SyntaxError, "SyntaxError");
EXCEPTION(FormatError, "FormatError");
EXCEPTION(NotImplementedError, "NotImplementedError");
EXCEPTION(DivideByZeroError, "DivideByZeroError");
EXCEPTION(AuthorError, "AuthorError");
EXCEPTION(IOError, "IOError");
EXCEPTION(ImportError, "ImportError");
EXCEPTION(EnvironmentError, "EnvironmentError");
