#include <string>
#include <iostream>

#include "debug.hpp"
#include "colour.hpp"

#include <stdarg.h>


#define MSGLEN 300
int DEBUG_MODE = 0;

void debug_message(const char *fmt, ...)
{

  if (!DEBUG_MODE)
    return;

  char message[MSGLEN] = {0};
  
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(message, MSGLEN-1, fmt, ap);
  va_end(ap);

  std::cerr << DIM << message << RESET;

}
