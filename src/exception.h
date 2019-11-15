#pragma once

#define ERROR_CHECK if (Exception_check()) return NULL;

void assert_or_error(int condition, const char *source, const char *fmt, ...);
int Exception_check();
void Exception_reset();
void Exception_print();
void Exception_raise(const char *message, const char *traceback);
