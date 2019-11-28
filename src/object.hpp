#pragma once
#include "atom.hpp"


class LispObject {
  private:
    long value_int;
    double value_float;
    std::string value_string;
};
