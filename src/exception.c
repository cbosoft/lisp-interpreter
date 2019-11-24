#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "colour.h"
#include "exception.h"
#include "types.h"

#define ERRLEN 256

enum EXCEPTION_STATUS {
  EXCEPTION_NONE,
  EXCEPTION_ERROR
};

// TODO: make struct?
static int exception_condition = EXCEPTION_NONE;
static char *exception_type = NULL;
static char *exception_message = NULL;
static char *exception_in = NULL;
static LispObject *exception_faulty_object = NULL;
static int exception_errno_set = 0;




// Raise an exception
void Exception_raise(const char *type, const char *in, LispObject *faulty_object, const char *message)
{
  exception_type = strdup(type);

  exception_message = strdup(message);

  exception_in = in != NULL ? strdup(in) : NULL;

  exception_faulty_object = faulty_object;

  exception_condition = EXCEPTION_ERROR;
}




// Raise exception, formatting message
void Exception_raisef(const char *type, const char *in, LispObject *faulty_object, const char *message_fmt, ...)
{
  char message[ERRLEN] = {0};

  va_list ap;

  va_start(ap, message_fmt);
  vsnprintf(message, ERRLEN-1, message_fmt, ap);
  va_end(ap);

  Exception_raise(type, in, faulty_object, message);
}




// Check if an exception is waiting to be shown
void Exception_set_errno(int v)
{
  exception_errno_set = v;
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
  exception_in = NULL;
  exception_faulty_object = NULL;
}




// print out current exception, and clear it.
void Exception_print()
{

  if (!Exception_check())
    return;

  fprintf(stderr, ""FG_RED"%s"RESET" raised: %s\n", 
      exception_type!=NULL ? exception_type : "Exception", 
      exception_message);

  if (exception_in != NULL)
    fprintf(stderr, "while running "FG_BLUE"%s"RESET"\n", exception_in);

  if (exception_errno_set)
    fprintf(stderr, "because "FG_YELLOW"(%d) %s"RESET"\n", errno, strerror(errno));

  if (exception_faulty_object != NULL)
    fprintf(stderr, "Problematic code from "FG_RED"%s (L:%d, C:%d)"RESET"\n", 
        exception_faulty_object->file, 
        exception_faulty_object->line,
        exception_faulty_object->col);


  Exception_reset();
}
