#pragma once

#include <string>
#include "../printable/printable.hpp"


class LispSymbol : virtual public Printable{
  private:
    std::string name;
  public:
    LispSymbol()
    {
      // do nothing
    }

    LispSymbol(std::string name)
      : name(name)
    {
      // do nothing
    }

    const std::string get_name() const
    {
      return this->name;
    }

    const std::string repr() const override
    {
      return this->name;
    }

    const std::string str() const override
    {
      return this->name;
    }
};
