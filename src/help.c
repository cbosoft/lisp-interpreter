#include <stdio.h>

#include "help.h"
#include "version.h"

void display_splash()
{
  FILE *fp = stderr;

  fprintf(fp, "crisp v"VERSION"\n");
}

void display_help()
{
  // no args = REPL
  // -f --file         execute commands in file
  // -c --command      execute command
  // -v --version      print interpreter version
  // -i --interactive  drop to REPL after executing $commands or $file
  // -h --help         print help screen
  // -d --debug        print more information
}
