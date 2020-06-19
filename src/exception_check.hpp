#include "types.hpp"
#include "exception.hpp"

void type_check_any(LispObject_ptr obj, std::list<LispObject_Type> types, std::string in, std::string arg);
void type_check_one(LispObject_ptr obj, LispObject_Type type, std::string in, std::string arg);
void type_check_atom(LispObject_ptr obj, LispAtom_Type type, std::string in, std::string arg);
void type_check_symbol(LispObject_ptr obj, LispEnvironment_Type type, std::string in, std::string arg, LispEnvironment_ptr env);

void narg_check(LispList_ptr arg, int expected_nargs, std::string in, std::string argnames);
void narg_check_min(LispList_ptr arg, int expected_nargs, std::string in, std::string argnames);
void narg_check_max(LispList_ptr arg, int expected_nargs, std::string in, std::string argnames);
