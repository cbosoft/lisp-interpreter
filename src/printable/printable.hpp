#pragma once

#include <string>
#include "../util/exception.hpp"

class Printable {
  public:
    Printable(){};
    virtual const std::string repr() const
    {
      throw NotImplementedError("In Printable::repr(): virtual function not implemented: intended to be used by sub class.");
    }

    virtual const std::string str() const
    {
      throw NotImplementedError("In Printable::str(): virtual function not implemented: intended to be used by sub class.");
    }

    void print()
    {
      this->_print(); std::cout << std::endl;
    };

    virtual void _print()
    {
      std::cout << this->repr() << " ";
    }
};
