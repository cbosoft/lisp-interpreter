#pragma once

enum LISPOBJECT_TYPE {
  LISPOBJECT_STRING,
  LISPOBJECT_INT,
  LISPOBJECT_FLOAT,
  LISPOBJECT_BOOL,
  LISPOBJECT_LIST,
};

typedef struct LispObject LispObject;
struct LispObject {
  char *value_string;
  int value_int;
  double value_float;
  int value_bool;

  int type;

  char *symbol_name;
  LispObject *symbol_value;

  LispObject *list_next;
  LispObject *list_child;
};

LispObject *new_string_object(char *value);
LispObject *new_int_object(int value);
LispObject *new_float_object(double value);
LispObject *new_bool_object(int value);
LispObject *new_list_object();
LispObject *new_object_guess_type(char *s);

void add_object_to_list(LispObject *list, LispObject *toadd);
LispObject *pop(LispObject *list);
LispObject *pop_index(LispObject *list, int index);
void LispObject_assign_value(LispObject *dest, LispObject *source);
void LispObject_print(LispObject *o);
void LispObject_free(LispObject *root);
