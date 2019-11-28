#pragma once
#include "types.h"

LispObject *add(LispListElement *arg, LispEnvironment *env);
LispObject *subtract(LispListElement *arg, LispEnvironment *env);
LispObject *multiply(LispListElement *arg, LispEnvironment *env);
LispObject *divide(LispListElement *arg, LispEnvironment *env);
LispObject *quote(LispListElement *arg, LispEnvironment *env);
LispObject *define(LispListElement *arg, LispEnvironment *env);
