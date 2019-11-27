#include <stdlib.h>
#include "types.h"

long truth = 1;
LispAtom t_atom = {&truth, NULL, NULL};
LispObject nil = {0};
LispObject t = { &t_atom, NULL, NULL, LISPOBJECT_ATOM, 0, 0, NULL};
