#include <iostream>

#include "version.hpp"
#include "help.hpp"


void display_splash()
{
  std::cout << EXE_NAME << "v" << VERSION << std::endl;
}



void display_help()
{
  std::cout 
    << EXE_NAME << "v" << VERSION << "\n"
    << "\n"
    << "  Usage:\n"
    << "    crisp [-i] [-d] [-c <command>] [<file> ...]\n"
    << "    crisp -h\n"
    << "\n"
    << "  Options:\n"
    << "    -i | --interactive        Drop to REPL after running command/file.\n"
    << "    -d | --debug              Show a bunch of debugging messages.\n"
    << "    -c | --command <command>  Execute command <command>.\n"
    << "    -h | --help               Print help and exit.\n"
    ;
  exit(0);
}
