#include <string>
#include <iostream>

#include "debug.hpp"
#include "colour.hpp"



int DEBUG_MODE = 0;

void debug_message(std::string s)
{

  if (!DEBUG_MODE)
    return;

  std::cerr << DIM << s << RESET << std::endl;

}
