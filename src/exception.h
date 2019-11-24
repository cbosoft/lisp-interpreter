#pragma once

#include "types.h"

#define ASSERT_OR_ERROR(COND, TYPE, IN, OBJ, RET, FMT, ...) \
  if (!(COND)) {\
    Exception_raisef(TYPE, IN, OBJ, FMT, ##__VA_ARGS__);\
    return RET;\
  }

#define ASSERT_OR_RAISE(COND, TYPE, IN, OBJ, FMT, ...) \
  if (!(COND)) {\
    Exception_raisef(TYPE, IN, OBJ, FMT, ##__VA_ARGS__);\
  }

#define ASSERT_OR_ERROR_WITH_ERRNO(COND, TYPE, IN, OBJ, RET, FMT, ...) \
  if (!(COND)) {\
    Exception_raisef(TYPE, IN, OBJ, FMT, ##__VA_ARGS__);\
    Exception_set_errno(1);\
    return RET;\
  }

#define ASSERT_OR_RAISE_WITH_ERRNO(COND, TYPE, IN, OBJ, FMT, ...) \
  if (!(COND)) {\
    Exception_raisef(TYPE, IN, OBJ, FMT, ##__VA_ARGS__);\
    Exception_set_errno(1);\
  }

#define ERROR_CHECK if (Exception_check()) return NULL;
#define ERROR_CHECK_RV(RET) if (Exception_check()) return RET;

int Exception_check();
void Exception_reset();
void Exception_print();
void Exception_set_errno(int v);
void Exception_raise(const char *type, const char *in, LispObject *faulty_object, const char *message);
void Exception_raisef(const char *type, const char *in, LispObject *faulty_object, const char *message_fmt, ...);
