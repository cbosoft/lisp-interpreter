#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "colour.h"
#include "exception.h"

#define ERRLEN 256

void assert_or_error(int condition, const char *source, const char *fmt, ...)
{
  if (condition)
    return;

  char message[ERRLEN] = {0};
  
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(message, ERRLEN-1, fmt, ap);
  va_end(ap);

  fprintf(stderr, BG_RED"Error"RESET" in "FG_BLUE"%s"RESET": %s\n", source, message);
  
  exit(1);
}
