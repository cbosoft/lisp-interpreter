#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "colour.h"

int DEBUG_MODE = 0;

#define MSGLEN 256


void debug_message(const char *fmt, ...)
{

  if (!DEBUG_MODE)
    return;

  char message[MSGLEN] = {0};
  
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(message, MSGLEN-1, fmt, ap);
  va_end(ap);

  fprintf(stderr, DIM"%s"RESET, message);

}
