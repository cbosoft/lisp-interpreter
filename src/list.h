#pragma once
#include "types.h"

int LispList_count(LispListElement *l);
LispListElement * LispList_new_element();
void LispList_add_object_to_list(LispListElement *list, LispObject *toadd);
