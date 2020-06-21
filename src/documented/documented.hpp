#pragma once
#include <string>

class Documented {
  public:
    virtual const std::string get_doc() const
    {
      return "Documentation for this object not available.";
    }
};
