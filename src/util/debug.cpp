#include <string>
#include <iostream>

#include "debug.hpp"
#include "colour.hpp"



int DEBUG_MODE = 0;
static int tablvl = 0;

void debug_message(std::string s)
{

  if (!DEBUG_MODE)
    return;

  for (int i = 0; i < tablvl; i++) {
    std::cerr << "  ";
  }

  std::cerr << DIM << FG_YELLOW << s << RESET << std::endl;

}


void inc_tab()
{
  tablvl++;
}


void dec_tab()
{
  tablvl--;
}
