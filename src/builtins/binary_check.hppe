#pragma once
#define BINARY_CHECK \
      narg_check(arg, 2, this->name, "left right");\
      LispObject_ptr left_obj = arg->next(true);\
      type_check_one(left_obj, LISPOBJECT_ATOM, this->name, "left");\
      LispAtom_ptr left_atom = left_obj->get_value_atom();\
      LispObject_ptr right_obj = arg->next();\
      type_check_one(right_obj, LISPOBJECT_ATOM, this->name, "right");\
      LispAtom_ptr right_atom = right_obj->get_value_atom();
