#include <iostream>

#include "../version.hpp"
#include "help.hpp"
#include "colour.hpp"


void display_splash()
{
  std::cout << std::endl << "  " << BOLD << EXE_NAME << RESET << " " << VERSION << std::endl << std::endl;
}



void display_help()
{
  display_splash();

  std::cout 
    << "  Usage:\n"
    << "    crisp [-i] [-d] [-c <command>] [<file> ...]\n"
    << "    crisp -h\n"
    << "\n"
    << "  Options:\n"
    << "    -i | --interactive        Drop to REPL after running command/file.\n"
    << "    -d | --debug              Show a bunch of debugging messages.\n"
    << "    -c | --command <command>  Execute command <command>.\n"
    << "    -h | --help               Print help and exit.\n"
    << "\n"
    ;
  exit(0);
}
