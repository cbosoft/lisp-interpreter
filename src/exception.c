#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "colour.h"
#include "exception.h"

#define ERRLEN 256

enum EXCEPTION_STATUS {
  EXCEPTION_NONE,
  EXCEPTION_ERROR
};

static int exception_condition = EXCEPTION_NONE;
static char *exception_message = NULL;
static char *exception_traceback = NULL;




// Raise an exception
void Exception_raise (const char *message, const char *traceback)
{
  exception_message = strdup(message);

  if (traceback != NULL)
    exception_traceback = strdup(traceback);
  else
    exception_traceback = NULL;

  exception_condition = EXCEPTION_ERROR;
}




// Check if an exception is waiting to be shown
int Exception_check()
{
  return exception_condition == EXCEPTION_ERROR;
}




// Reset exception
void Exception_reset()
{
  exception_condition = EXCEPTION_NONE;
  exception_message = NULL;
  exception_traceback = NULL;
}




// print out current exception, and clear it.
void Exception_print()
{

  if (!Exception_check())
    return;

  fprintf(stderr, BG_RED"Exception"RESET": %s\n", exception_message);

  if (exception_traceback != NULL)
    fprintf(stderr, "in: "FG_BLUE"%s"RESET"\n", exception_traceback);

  Exception_reset();
}




// assert a condition is true, otherwise raise an exception.
void assert_or_error(int condition, const char *source, const char *fmt, ...)
{
  if (condition)
    return;

  char message[ERRLEN] = {0};
  
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(message, ERRLEN-1, fmt, ap);
  va_end(ap);

  Exception_raise((const char *)message, source);

}




// assert a condition is true, otherwise raise an exception, including information from errno
void assert_or_error_with_errno(int condition, const char *source, const char *fmt, ...)
{
  if (condition)
    return;

  char message[ERRLEN] = {0};
  
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(message, ERRLEN-1, fmt, ap);
  va_end(ap);

  exception_errno_set = 1;
  Exception_raise((const char *)message, source);
}
