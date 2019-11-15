#pragma once

enum LISPOBJECT_TYPE {
  LISPOBJECT_STRING,
  LISPOBJECT_INT,
  LISPOBJECT_FLOAT,
  LISPOBJECT_BOOL,
  LISPOBJECT_LIST,
  LISPOBJECT_SYMBOL
};

typedef struct LispObject LispObject;
struct LispObject {
  char *value_string;
  int value_int;
  double value_float;
  int value_bool;

  int type;

  char *symbol_name;
  LispObject *value_symbol;

  LispObject *list_next;
  LispObject *list_child;
};

LispObject *LispObject_new_string(char *value);
LispObject *LispObject_new_int(int value);
LispObject *LispObject_new_float(double value);
LispObject *LispObject_new_bool(int value);
LispObject *LispObject_new_list();
LispObject *LispObject_new_guess_type(char *s);
void LispObject_add_object_to_list(LispObject *list, LispObject *toadd);
void LispObject_assign_value(LispObject *dest, LispObject *source);
void LispObject_print(LispObject *o);
void LispObject_free(LispObject *root);
LispObject *LispObject_deepcopy(LispObject *o);
LispObject *LispObject_copy(LispObject *o);
char *LispObject_type(LispObject *o);
int LispObject_list_size(LispObject *o);
char *LispObject_repr(LispObject *o);
