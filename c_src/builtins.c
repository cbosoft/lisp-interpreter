#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "object.h"
#include "builtins.h"
#include "exception.h"
#include "environment.h"
#include "debug.h"
#include "types.h"
#include "function.h"
#include "list.h"
#include "eval.h"
#include "gc.h"
#include "import.h"
#include "atom.h"

extern LispObject nil;
extern LispObject t;


#define TOUCH(LE) if (LE!=NULL) {};



// math.c
extern LispBuiltin add_obj;
extern LispBuiltin subtract_obj;
extern LispBuiltin multiply_obj;
extern LispBuiltin divide_obj;



// Returns args as output without change
LispObject *quote(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION QUOTE\n");

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "quote", NULL, NULL, "Function takes 1 argument (got %d).", nargs);

  TOUCH(env);
  return arg->value;
}
LispBuiltin quote_obj = {&quote, LISPBUILTIN_LAZY};




// Create an entry in the local environment's function table.
// Usage: (define name arglist &rest body)
// Arguments:
//   name            string or symbol name of the function
//   arglist         list of string or symbol names of the function arguments,
//   body            rest of the arguments make up the list of contents in the function body
LispObject *define(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DEFINE\n");

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 3, "ArgumentError", "define", NULL, NULL, "Function takes 3 arguments: name, arglist, body (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *name = arg->value;
  ASSERT_OR_ERROR(name->type == LISPOBJECT_SYMBOL, "TypeError", "define", name, NULL, "Argument has wrong type: name should be a Symbol, not %s", LispObject_type(name));
  debug_message("AFTER NAME TYPE CHECK\n");

  if (!LispEnvironment_get(env, name->value_symbol, NULL, NULL, NULL)) {
    LispEnvironment_del_variable(env, name->value_symbol);
  }
  Exception_reset();

  LispObject *arglist = arg->next->value;
  ASSERT_OR_ERROR(arglist->type == LISPOBJECT_LIST, "TypeError", "define", arglist, NULL, "Argument has wrong type: arglist should be a List, not %s", LispObject_type(arglist));
  debug_message("AFTER ARGLIST CHECK\n");

  LispListElement *i;
  for (i = arglist->value_list; i->value != NULL; i = i->next) {
    ASSERT_OR_ERROR(i->value->type == LISPOBJECT_SYMBOL, "TypeError", "define", i->value, NULL, "Argument has wrong type: arglist items should be Symbols, not %s", LispObject_type(i->value));
  }
  debug_message("AFTER ARGLIST_ITEMS CHECK\n");

  LispListElement *body = arg->next->next;
  LispObject *content = LispObject_new_list();
  for (LispListElement *i = body; i->value != NULL; i = i->next) {
    LispList_add_object_to_list(content->value_list, i->value);
  }
  LispFunction *lfunc = LispFunction_new(arglist->value_list, content);
  LispEnvironment_add_variable(env, name->value_symbol, lfunc, NULL, NULL);
  return name;
}
LispBuiltin define_obj = {&define, LISPBUILTIN_LAZY};




// Create an entry in the local environment's variable table.
// Usage: (defvar name value)
// Arguments:
//   name            string or symbol name of the function
//   value           value of the variable
LispObject *defvar(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION DEFVAR\n");

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "defvar", NULL, NULL,  "Function takes 2 arguments: name, value (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *name = arg->value;
  ASSERT_OR_ERROR(name->type == LISPOBJECT_SYMBOL, "TypeError", "defvar", name, NULL, "Argument has wrong type: name should be a Symbol, not %s", LispObject_type(name));
  ERROR_CHECK;
  debug_message("AFTER NAME TYPE CHECK\n");

  if (!LispEnvironment_get(env, name->value_symbol, NULL, NULL, NULL)) {
    LispEnvironment_del_variable(env, name->value_symbol);
  }
  Exception_reset();

  LispObject *value = arg->next->value;
  LispEnvironment_add_variable(env, name->value_symbol, NULL, NULL, value);

  return value;
}
LispBuiltin defvar_obj = {&defvar, LISPBUILTIN_LAZY};




// Get number of elements in a list
// Usage:
//   (count list)
// Arguments:
//   
LispObject *count(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION count\n");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "define", NULL, NULL, "Function takes 1 arguments: list (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *list = arg->value;
  ASSERT_OR_ERROR(list->type == LISPOBJECT_LIST, "TypeError", "define", list, NULL, "Argument has wrong type: list should be a List, not %s", LispObject_type(list));
  debug_message("AFTER LIST CHECK\n");

  int nelems = LispList_count(list->value_list); // obj->list_child is the real start of the list
  LispObject *rv = LispObject_new_int(nelems);

  return rv;
}
LispBuiltin count_obj = {&count, LISPBUILTIN_LAZY};




// If $condition, return eval($thing), else return eval(otherthing)
LispObject *lisp_if(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION lisp_if");

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 3, "ArgumentError", "lisp_if", NULL, NULL, "Function takes 3 arguments: condition, result-if-true, result-else (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");
  
  LispObject *condition = eval(arg->value, env);
  ERROR_CHECK;

  int condition_value = LispObject_is_truthy(condition);
  ERROR_CHECK;
  debug_message("AFTER CONDITION EVAL\n");

  LispObject *rv = NULL;

  if (condition_value) {
    LispObject *value_if_true = arg->next->value;
    rv = eval(value_if_true, env);
    ERROR_CHECK;
  }
  else {
    LispObject *value_else = arg->next->next->value;
    rv = eval(value_else, env);
    ERROR_CHECK;
  }

  return rv;
}
LispBuiltin lisp_if_obj = {&lisp_if, LISPBUILTIN_LAZY};




// compare: using the object comparison funcs
#define COMPARE(OPNAME) \
  debug_message("BUILTIN FUNCTION lisp_comparison\n");\
  TOUCH(env);\
  int nargs = LispList_count(arg);\
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "lisp_comparison", NULL, NULL, "Function takes 2 arguments: left, right (got %d).", nargs);\
  LispObject \
    *left = arg->value, \
    *right = arg->next->value;\
  int rv_value = OPNAME(left, right);\
  ERROR_CHECK;\
  return rv_value ? &t : &nil;

LispObject *lisp_gt(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_gt)}
LispObject *lisp_ge(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_ge)}
LispObject *lisp_lt(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_lt)}
LispObject *lisp_le(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_le)}
LispObject *lisp_eq(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_eq)}
LispObject *lisp_or(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_or)}
LispObject *lisp_and(LispListElement *arg, LispEnvironment *env) {COMPARE(LispObject_and)}
LispBuiltin lisp_gt_obj = {&lisp_gt, LISPBUILTIN_GREEDY};
LispBuiltin lisp_ge_obj = {&lisp_ge, LISPBUILTIN_GREEDY};
LispBuiltin lisp_lt_obj = {&lisp_lt, LISPBUILTIN_GREEDY};
LispBuiltin lisp_le_obj = {&lisp_le, LISPBUILTIN_GREEDY};
LispBuiltin lisp_eq_obj = {&lisp_eq, LISPBUILTIN_GREEDY};
LispBuiltin lisp_or_obj = {&lisp_or, LISPBUILTIN_GREEDY};
LispBuiltin lisp_and_obj = {&lisp_and, LISPBUILTIN_GREEDY};




// pop (car): return first object in a list
LispObject *pop(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION POP/CDR\n");

  TOUCH(env);

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "pop", NULL, NULL, "Function takes 1 arguments: list (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *list = arg->value;
  if (list->type == LISPOBJECT_NIL)
    return &nil;
  ASSERT_OR_ERROR(list->type == LISPOBJECT_LIST, "TypeError", "pop", list, NULL, "list must be a List, not %s", LispObject_type(list));
  debug_message("AFTER TYPE CHECK\n");

  int list_len = LispList_count(list->value_list);
  if (list_len == 0)
    return &nil;

  return list->value_list->value;
}
LispBuiltin pop_obj = {&pop, LISPBUILTIN_GREEDY};




// rest (AKA cdr): return the list elements after the first i.e. !pop
LispObject *rest(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION REST/CAR");

  TOUCH(env);

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "rest", NULL, NULL, "Function takes 1 arguments: list (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *list = arg->value;
  if (list->type == LISPOBJECT_NIL)
    return &nil;
  ASSERT_OR_ERROR(list->type == LISPOBJECT_LIST, "TypeError", "rest", list, NULL, "list must be a List, not %s", LispObject_type(list));
  debug_message("AFTER TYPE CHECK\n");

  int list_len = LispList_count(list->value_list);
  if (list_len == 0)
    return &nil;

  LispObject *rest = calloc(1, sizeof(LispObject));
  rest->type = LISPOBJECT_LIST;
  rest->value_list = list->value_list->next;
  
  return rest;
}
LispBuiltin rest_obj = {&rest, LISPBUILTIN_GREEDY};




// print: print a string representation of an object
LispObject *print(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION PRINT");

  TOUCH(env);

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "print", NULL, NULL, "Function takes 1 arguments: (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *o = arg->value;

  LispObject_print(o);

  return NULL;
}
LispBuiltin print_obj = {&print, LISPBUILTIN_GREEDY};




// Load a file
LispObject *load_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION LOAD-FILE");

  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "load-file", NULL, NULL, "Function takes 1 arguments: (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *filename_obj = arg->value;
  ASSERT_OR_ERROR(filename_obj->type == LISPOBJECT_ATOM, "TypeError", "load-file", filename_obj, NULL, "filename must be a Atomistic, not %s", LispObject_type(filename_obj));
  debug_message("AFTER TYPE CHECK\n");

  LispAtom *filename_atm = LispAtom_cast_as(filename_obj->value_atom, LISPATOM_STRING);
  ERROR_CHECK;

  eval_file(filename_atm->value_string, env);
  ERROR_CHECK;

  return &nil;
}
LispBuiltin load_file_obj = {&load_file, LISPBUILTIN_GREEDY};




// (exit rv): exit with error code $rv.
LispObject *lisp_exit(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION EXIT");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs < 2, "ArgumentError", "exit", NULL, NULL, "Function takes at most 1 argument: (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  long rv = 0;

  if (nargs) {
    LispObject *rv_obj = arg->value;
    ASSERT_OR_ERROR(rv_obj->type == LISPOBJECT_ATOM, "TypeError", "exit", rv_obj, NULL, "rv must be Atomistic, not %s", LispObject_type(rv_obj));

    LispAtom *rv_atm = LispAtom_cast_as(rv_obj->value_atom, LISPATOM_INT);
    ERROR_CHECK;

    rv = (*rv_atm->value_int);
  }

  exit(rv);
}
LispBuiltin exit_obj = {&lisp_exit, LISPBUILTIN_GREEDY};




// read contents of a file
// Usage:
//   (read-file path [mode])
// Arguments:
//   path    Path to file to open
LispObject *read_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION READ_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "read-file", NULL, NULL, "Function takes 1 argument (path): (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *pathobj = arg->value;
  ASSERT_OR_ERROR(pathobj->type == LISPOBJECT_ATOM, "TypeError", "read-file", pathobj, NULL, "path argument must be a Atomistic, not %s", LispObject_type(pathobj));
  debug_message("AFTER PATH TYPE CHECK\n");

  LispAtom *pathatm = LispAtom_cast_as(pathobj->value_atom, LISPATOM_STRING);

  FILE *fp = fopen(pathatm->value_string, "r");
  ASSERT_OR_ERROR_WITH_ERRNO(fp != NULL, "IOError", "read-file", NULL, NULL, "Could not open file '%s'", pathatm->value_string);

  int rv = fseek(fp, 0, SEEK_END);
  ASSERT_OR_RAISE_WITH_ERRNO(rv == 0, "IOError", "read-file", NULL, "Failed to get size of file.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }
  long length = ftell(fp);

  rv = fseek(fp, 0, SEEK_SET);
  ASSERT_OR_RAISE_WITH_ERRNO(rv == 0, "IOError", "read-file", NULL, "Failed to return to start of file.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  char *contents = malloc((length+1)*sizeof(char));
  ASSERT_OR_RAISE_WITH_ERRNO(contents != NULL, "IOError", "read-file", NULL, "Failed to allocate memory to hold file contents.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  rv = fread(contents, 1, length, fp);
  ASSERT_OR_RAISE_WITH_ERRNO(rv != -1, "IOError", "read-file", NULL, "Failed to read file into memory.");
  if (Exception_check()) {
    fclose(fp);
    return NULL;
  }

  contents[length] = '\0';

  fclose(fp);


  return LispObject_new_string(contents);
}
LispBuiltin read_file_obj = {&read_file, LISPBUILTIN_GREEDY};




// write-file
LispObject *write_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION WRITE_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "write-file", NULL, NULL, "Function takes 2 arguments (path content): (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *pathobj = arg->value;
  ASSERT_OR_ERROR(pathobj->type == LISPOBJECT_ATOM, "TypeError", "write-file", pathobj, NULL, "path argument must be a Atomistic, not %s", LispObject_type(pathobj));
  ERROR_CHECK;
  debug_message("AFTER PATH TYPE CHECK\n");

  LispAtom *pathatm = LispAtom_cast_as(pathobj->value_atom, LISPATOM_STRING);

  LispObject *contentobj = arg->next->value;
  ASSERT_OR_ERROR(contentobj->type == LISPOBJECT_ATOM, "TypeError", "write-file", contentobj, NULL, "content argument must be a Atomistic, not %s", LispObject_type(contentobj));
  ERROR_CHECK;
  debug_message("AFTER CONTENT TYPE CHECK\n");

  LispAtom *contentatm = LispAtom_cast_as(contentobj->value_atom, LISPATOM_STRING);

  FILE *fp = fopen(pathatm->value_string, "w");
  ASSERT_OR_ERROR_WITH_ERRNO(fp != NULL, "IOError", "write-file", NULL, NULL, "Could not open file '%s' for writing.", pathatm->value_string);

  int rv = fprintf(fp, "%s", contentatm->value_string);
  ASSERT_OR_RAISE(rv > 0, "IOError", "write-file", NULL, "Could not write file '%s'", pathatm->value_string);
  fclose(fp);

  ERROR_CHECK;

  return LispObject_new_string(contentatm->value_string);
}
LispBuiltin write_file_obj = {&write_file, LISPBUILTIN_GREEDY};




// append-file
LispObject *append_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION WRITE_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "write-file", NULL, NULL, "Function takes 2 arguments (path content): (got %d).", nargs);
  ERROR_CHECK;
  debug_message("AFTER NARGS CHECK\n");

  LispObject *pathobj = arg->value;
  ASSERT_OR_ERROR(pathobj->type == LISPOBJECT_ATOM, "TypeError", "write-file", pathobj, NULL, "path argument must be a Atomistic, not %s", LispObject_type(pathobj));
  ERROR_CHECK;
  debug_message("AFTER PATH TYPE CHECK\n");

  LispAtom *pathatm = LispAtom_cast_as(pathobj->value_atom, LISPATOM_STRING);

  LispObject *contentobj = arg->next->value;
  ASSERT_OR_ERROR(contentobj->type == LISPOBJECT_ATOM, "TypeError", "write-file", contentobj, NULL, "content argument must be a Atomistic, not %s", LispObject_type(contentobj));
  ERROR_CHECK;
  debug_message("AFTER CONTENT TYPE CHECK\n");

  LispAtom *contentatm = LispAtom_cast_as(contentobj->value_atom, LISPATOM_STRING);

  FILE *fp = fopen(pathatm->value_string, "a");
  ASSERT_OR_ERROR_WITH_ERRNO(fp != NULL, "IOError", "write-file", NULL, NULL, "Could not open file '%s' for writing.", pathatm->value_string);

  int rv = fprintf(fp, "%s", contentatm->value_string);
  ASSERT_OR_RAISE(rv > 0, "IOError", "write-file", NULL, "Could not write file '%s'", pathatm->value_string);
  fclose(fp);

  ERROR_CHECK;

  return LispObject_new_string(contentatm->value_string);
}
LispBuiltin append_file_obj = {&append_file, LISPBUILTIN_GREEDY};




// (import module)
LispObject *import_module(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION IMPORT-MODULE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "import-module", NULL, NULL, "Function takes 1 arguments (name): (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *name = arg->value;
  ASSERT_OR_ERROR(name->type == LISPOBJECT_SYMBOL, "TypeError", "import-module", name, NULL, "name to import should be a Symbol, not %s", LispObject_type(name));
  debug_message("AFTER NAME TYPE CHECK\n");

  char *path = search(name->value_symbol);
  ERROR_CHECK;

  debug_message("SEARCH RETURNED PATH %s\n", path);

  eval_file(path, env);
  ERROR_CHECK;

  return &nil;
}
LispBuiltin import_module_obj = {&import_module, LISPBUILTIN_LAZY};




// (eval-file path)
LispObject *lisp_eval_file(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION EVAL_FILE");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "eval-file", NULL, NULL, "Function takes 1 arguments (path): (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *path = arg->value;
  ASSERT_OR_ERROR(path->type == LISPOBJECT_ATOM, "TypeError", "eval-file", path, NULL, "path should be a Atom, not %s", LispObject_type(path));
  debug_message("AFTER PATH TYPE CHECK\n");

  LispAtom *path_atom = LispAtom_cast_as(path->value_atom, LISPATOM_STRING);

  eval_file(path_atom->value_string, env);
  ERROR_CHECK;

  return &nil;
}
LispBuiltin lisp_eval_file_obj = {&lisp_eval_file, LISPBUILTIN_GREEDY};




// (concat list1 list2)
// Concatenate two lists.
LispObject *concat(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION CONCAT");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "concat", NULL, NULL, "Function takes 2 arguments (list1 list2): (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *list1 = arg->value;
  ASSERT_OR_ERROR(list1->type == LISPOBJECT_LIST, "TypeError", "eval-file", list1, NULL, "list1 should be a List, not %s", LispObject_type(list1));
  debug_message("AFTER LIST1 TYPE CHECK\n");

  LispObject *list2 = arg->next->value;
  ASSERT_OR_ERROR(list2->type == LISPOBJECT_LIST, "TypeError", "eval-file", list2, NULL, "list2 should be a List, not %s", LispObject_type(list2));
  debug_message("AFTER LIST2 TYPE CHECK\n");

  LispListElement *i;
  LispObject *rv = LispObject_new_list();
  ERROR_CHECK;

  for (i = list1->value_list; i->value != NULL; i = i->next)
    LispList_add_object_to_list(rv->value_list, i->value);

  for (i = list2->value_list; i->value != NULL; i = i->next)
    LispList_add_object_to_list(rv->value_list, i->value);

  return rv;
}
LispBuiltin concat_obj = {&concat, LISPBUILTIN_GREEDY};




// (append list obj)
// Add object to list
LispObject *append(LispListElement *arg, LispEnvironment *env)
{
  debug_message("BUILTIN FUNCTION APPEND");

  TOUCH(env);
  int nargs = LispList_count(arg);
  ASSERT_OR_ERROR(nargs == 2, "ArgumentError", "concat", NULL, NULL, "Function takes 2 arguments (list append): (got %d).", nargs);
  debug_message("AFTER NARGS CHECK\n");

  LispObject *list = arg->value;
  ASSERT_OR_ERROR(list->type == LISPOBJECT_LIST, "TypeError", "eval-file", list, NULL, "list should be a List, not %s", LispObject_type(list));
  debug_message("AFTER LIST TYPE CHECK\n");

  LispListElement *i;
  LispObject *rv = LispObject_new_list();
  ERROR_CHECK;

  for (i = list->value_list; i->value != NULL; i = i->next)
    LispList_add_object_to_list(rv->value_list, i->value);

  LispObject *obj = arg->next->value;
  LispList_add_object_to_list(rv->value_list, obj);

  return rv;
}
LispBuiltin append_obj = {&append, LISPBUILTIN_GREEDY};




// (map f (arg1[ arg2[ ...]]))
// LispObject *map(LispListElement *arg, LispEnvironment *env)
// {
//   debug_message("BUILTIN FUNCTION MAP");
// 
//   TOUCH(env);
//   int nargs = LispList_count(arg);
//   ASSERT_OR_ERROR(nargs == 1, "ArgumentError", "eval-file", NULL, NULL, "Function takes 1 arguments (path): (got %d).", nargs);
//   debug_message("AFTER NARGS CHECK\n");
// 
//   LispObject *path = arg->value;
//   ASSERT_OR_ERROR(path->type == LISPOBJECT_STRING, "TypeError", "eval-file", path, NULL, "path should be a String, not %s", LispObject_type(path));
//   debug_message("AFTER PATH TYPE CHECK\n");
// 
//   eval_file(path->value_string, env);
//   ERROR_CHECK;
// 
//   return &nil;
// }
// LispBuiltin mapf_obj = {&map, LISPBUILTIN_LAZY};




// builtins are enumerated here, and referred to in the global env setup
struct environment_var builtin_functions[] = {
  { "add", "+", NULL, &add_obj, NULL, NULL},
	{ "subtract", "-", NULL, &subtract_obj, NULL, NULL },
	{ "multiply", "*", NULL, &multiply_obj, NULL, NULL },
	{ "divide", "/", NULL, &divide_obj, NULL, NULL },
	{ "quote", NULL, NULL, &quote_obj, NULL, NULL },
	{ "define", NULL, NULL, &define_obj, NULL, NULL },
	{ "defvar", NULL, NULL, &defvar_obj, NULL, NULL },
	{ "count", NULL, NULL, &count_obj, NULL, NULL },
	{ "if", NULL, NULL, &lisp_if_obj, NULL, NULL },
	{ "gt", ">", NULL, &lisp_gt_obj, NULL, NULL },
	{ "ge", ">=", NULL, &lisp_ge_obj, NULL, NULL },
	{ "lt", "<", NULL, &lisp_lt_obj, NULL, NULL },
	{ "le", "<=", NULL, &lisp_le_obj, NULL, NULL },
	{ "eq", "=", NULL, &lisp_eq_obj, NULL, NULL },
	{ "or", "||", NULL, &lisp_or_obj, NULL, NULL },
	{ "and", "&&", NULL, &lisp_and_obj, NULL, NULL },
	{ "pop", "car", NULL, &pop_obj, NULL, NULL },
	{ "rest", "cdr", NULL, &rest_obj, NULL, NULL },
	{ "print", NULL, NULL, &print_obj, NULL, NULL },
	{ "load-file", NULL, NULL, &load_file_obj, NULL, NULL },
	{ "exit", NULL, NULL, &exit_obj, NULL, NULL },
	{ "read-file", NULL, NULL, &read_file_obj, NULL, NULL },
	{ "write-file", NULL, NULL, &write_file_obj, NULL, NULL },
	{ "append-file", NULL, NULL, &append_file_obj, NULL, NULL },
	{ "import-module", "import", NULL, &import_module_obj, NULL, NULL },
	{ "eval-file", NULL, NULL, &lisp_eval_file_obj, NULL, NULL },
	{ "concat", NULL, NULL, &concat_obj, NULL, NULL },
	{ "append", NULL, NULL, &append_obj, NULL, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL }
};
