#include <stdlib.h>
#include <stdio.h>

#include "gc.h"
#include "types.h"
#include "object.h"
#include "builtins.h"
#include "list.h"
#include "colour.h"
#include "exception.h"

#define RUN_TEST(TEST) if ((rc = TEST()) != 0) return rc;
#define FAIL(REASON) do {fprintf(stderr, "  -> "BOLD"%s"RESET" "BG_RED"FAILED"RESET" (%s)\n", testname, REASON); return 1; } while (0);
#define PASS fprintf(stderr, "  -> "BOLD"%s"RESET" "FG_GREEN"PASSED"RESET"\n", testname);

int builtins_float_arithmetic_test()
{

  fprintf(stderr, BOLD""FG_BLUE"FLOATING POINT ARITHMETIC"RESET"\n");

  double
    small_float_1 = 0.1,//(double)(rand() % 100),
    small_float_2 = 0.2,//(double)(rand() % 100),
    big_float_1 = 1e9,//(double)rand(), 
    big_float_2 = 1e-5;//(double)rand();

  LispObject 
    *small_float_obj1 = LispObject_new_float(small_float_1),
    *small_float_obj2 = LispObject_new_float(small_float_2),
    *big_float_obj1 = LispObject_new_float(big_float_1),
    *big_float_obj2 = LispObject_new_float(big_float_2),
    *zero = LispObject_new_float(0.0),
    *string_obj = LispObject_new_string("string");

  LispListElement 
    *small_args = LispList_new_element(), 
    *big_args = LispList_new_element(), 
    *type_error_args = LispList_new_element(), 
    *zero_error_args = LispList_new_element(),
    *type_error_args_swapped = LispList_new_element(), 
    *zero_error_args_swapped = LispList_new_element(),
    *too_many_args = LispList_new_element();

  LispList_add_object_to_list(small_args, small_float_obj1);
  LispList_add_object_to_list(small_args, small_float_obj2);

  LispList_add_object_to_list(big_args, big_float_obj1);
  LispList_add_object_to_list(big_args, big_float_obj2);

  LispList_add_object_to_list(type_error_args, small_float_obj1);
  LispList_add_object_to_list(type_error_args, string_obj);

  LispList_add_object_to_list(type_error_args_swapped, string_obj);
  LispList_add_object_to_list(type_error_args_swapped, small_float_obj1);

  LispList_add_object_to_list(zero_error_args, small_float_obj2);
  LispList_add_object_to_list(zero_error_args, zero);

  LispList_add_object_to_list(zero_error_args_swapped, zero);
  LispList_add_object_to_list(zero_error_args_swapped, small_float_obj2);

  LispList_add_object_to_list(too_many_args, small_float_obj1);
  LispList_add_object_to_list(too_many_args, small_float_obj2);
  LispList_add_object_to_list(too_many_args, small_float_obj2);

  char *testname = NULL;
  LispObject *res = NULL;




  // Arguments: (float, float)
  // Expectation: No exception should be raised, value should match.
  testname = "add (small args)";
  Exception_reset();
  res = add(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (small_float_1 + small_float_2)) FAIL("incorrect value computed");
  PASS;

  testname = "subtract (small args)";
  Exception_reset();
  res = subtract(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (small_float_1 - small_float_2)) FAIL("incorrect value computed");
  PASS;

  testname = "multiply (small args)";
  Exception_reset();
  res = multiply(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (small_float_1 * small_float_2)) FAIL("incorrect value computed");
  PASS;

  testname = "divide (small args)";
  Exception_reset();
  res = divide(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (small_float_1 / small_float_2)) FAIL("incorrect value computed");
  PASS;




  // Arguments: (float, float)
  // Expectation: No exception should be raised, value should match.
  testname = "add (big args)";
  Exception_reset();
  res = add(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (big_float_1 + big_float_2)) FAIL("incorrect value computed");
  PASS;

  testname = "subtract (big args)";
  Exception_reset();
  res = subtract(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (big_float_1 - big_float_2)) FAIL("incorrect value computed");
  PASS;

  testname = "multiply (big args)";
  Exception_reset();
  res = multiply(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_float != (big_float_1 * big_float_2)) FAIL("incorrect value computed");
  PASS;

  testname = "divide (big args)";
  Exception_reset();
  res = divide(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res->value_float != (big_float_1 / big_float_2)) FAIL("incorrect value computed");
  PASS;




  // Arguments: (float, string)
  // Expectation: All should raise exception.
  testname = "add (type error args 1)";
  Exception_reset();
  res = add(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "subtract (type error args 1)";
  Exception_reset();
  res = subtract(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "multiply (type error args 1)";
  Exception_reset();
  res = multiply(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "divide (type error args 1)";
  Exception_reset();
  res = divide(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;




  // Arguments: (string, int)
  // Expectation: All should raise exception.
  testname = "add (type error args 2)";
  Exception_reset();
  res = add(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "subtract (type error args 2)";
  Exception_reset();
  res = subtract(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "multiply (type error args 2)";
  Exception_reset();
  res = multiply(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "divide (type error args 2)";
  Exception_reset();
  res = divide(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;




  // Arguments: (int, 0)
  // Expectation: Divide should raise exception, none else.
  testname = "add (zero error args 1)";
  Exception_reset();
  res = add(zero_error_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "subtract (zero error args 1)";
  Exception_reset();
  res = subtract(zero_error_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "multiply (zero error args 1)";
  Exception_reset();
  res = multiply(zero_error_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "divide (zero error args 1)";
  Exception_reset();
  res = divide(zero_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;



  // Arguments: (0, int)
  // Expectation: None of them should raise exception.
  testname = "add (zero error args 2)";
  Exception_reset();
  res = add(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "subtract (zero error args 2)";
  Exception_reset();
  res = subtract(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "multiply (zero error args 2)";
  Exception_reset();
  res = multiply(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "divide (zero error args 2)";
  Exception_reset();
  res = divide(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;



  // Arguments: (int, int, int)
  // Expectation: All should raise exception.
  testname = "add (too many args 2)";
  Exception_reset();
  res = add(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "subtract (too many args 2)";
  Exception_reset();
  res = subtract(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "multiply (too many args 2)";
  Exception_reset();
  res = multiply(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "divide (too many args 2)";
  Exception_reset();
  res = divide(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  return 0;
}





int builtins_integer_arithmetic_test()
{

  fprintf(stderr, BOLD""FG_BLUE"INTEGER ARITHMETIC"RESET"\n");

  int 
    small_int_1 = rand() % 100,
    small_int_2 = rand() % 100,
    big_int_1 = rand(), 
    big_int_2 = rand();

  LispObject 
    *small_int_obj1 = LispObject_new_int(small_int_1),
    *small_int_obj2 = LispObject_new_int(small_int_2),
    *big_int_obj1 = LispObject_new_int(big_int_1),
    *big_int_obj2 = LispObject_new_int(big_int_2),
    *zero = LispObject_new_int(0),
    *string_obj = LispObject_new_string("string");

  LispListElement 
    *small_args = LispList_new_element(), 
    *big_args = LispList_new_element(), 
    *type_error_args = LispList_new_element(), 
    *zero_error_args = LispList_new_element(),
    *type_error_args_swapped = LispList_new_element(), 
    *zero_error_args_swapped = LispList_new_element(),
    *too_many_args = LispList_new_element();

  LispList_add_object_to_list(small_args, small_int_obj1);
  LispList_add_object_to_list(small_args, small_int_obj2);

  LispList_add_object_to_list(big_args, big_int_obj1);
  LispList_add_object_to_list(big_args, big_int_obj2);

  LispList_add_object_to_list(type_error_args, small_int_obj1);
  LispList_add_object_to_list(type_error_args, string_obj);

  LispList_add_object_to_list(type_error_args_swapped, string_obj);
  LispList_add_object_to_list(type_error_args_swapped, small_int_obj1);

  LispList_add_object_to_list(zero_error_args, small_int_obj2);
  LispList_add_object_to_list(zero_error_args, zero);

  LispList_add_object_to_list(zero_error_args_swapped, zero);
  LispList_add_object_to_list(zero_error_args_swapped, small_int_obj2);

  LispList_add_object_to_list(too_many_args, small_int_obj1);
  LispList_add_object_to_list(too_many_args, small_int_obj2);
  LispList_add_object_to_list(too_many_args, small_int_obj2);

  char *testname = NULL;
  LispObject *res = NULL;




  // Arguments: (int, int)
  // Expectation: No exception should be raised, value should match.
  testname = "add (small args)";
  Exception_reset();
  res = add(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (small_int_1 + small_int_2)) FAIL("incorrect value computed");
  PASS;

  testname = "subtract (small args)";
  Exception_reset();
  res = subtract(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (small_int_1 - small_int_2)) FAIL("incorrect value computed");
  PASS;

  testname = "multiply (small args)";
  Exception_reset();
  res = multiply(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (small_int_1 * small_int_2)) FAIL("incorrect value computed");
  PASS;

  testname = "divide (small args)";
  Exception_reset();
  res = divide(small_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (small_int_1 / small_int_2)) FAIL("incorrect value computed");
  PASS;




  // Arguments: (int, int)
  // Expectation: No exception should be raised, value should match.
  testname = "add (big args)";
  Exception_reset();
  res = add(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (big_int_1 + big_int_2)) FAIL("incorrect value computed");
  PASS;

  testname = "subtract (big args)";
  Exception_reset();
  res = subtract(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (big_int_1 - big_int_2)) FAIL("incorrect value computed");
  PASS;

  testname = "multiply (big args)";
  Exception_reset();
  res = multiply(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res == NULL) FAIL("returned null");
  if (res->value_int != (big_int_1 * big_int_2)) FAIL("incorrect value computed");
  PASS;

  testname = "divide (big args)";
  Exception_reset();
  res = divide(big_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  if (res->value_int != (big_int_1 / big_int_2)) FAIL("incorrect value computed");
  PASS;




  // Arguments: (int, string)
  // Expectation: All should raise exception.
  testname = "add (type error args 1)";
  Exception_reset();
  res = add(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "subtract (type error args 1)";
  Exception_reset();
  res = subtract(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "multiply (type error args 1)";
  Exception_reset();
  res = multiply(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "divide (type error args 1)";
  Exception_reset();
  res = divide(type_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;




  // Arguments: (string, int)
  // Expectation: All should raise exception.
  testname = "add (type error args 2)";
  Exception_reset();
  res = add(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "subtract (type error args 2)";
  Exception_reset();
  res = subtract(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "multiply (type error args 2)";
  Exception_reset();
  res = multiply(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "divide (type error args 2)";
  Exception_reset();
  res = divide(type_error_args_swapped, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;




  // Arguments: (int, 0)
  // Expectation: Divide should raise exception, none else.
  testname = "add (zero error args 1)";
  Exception_reset();
  res = add(zero_error_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "subtract (zero error args 1)";
  Exception_reset();
  res = subtract(zero_error_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "multiply (zero error args 1)";
  Exception_reset();
  res = multiply(zero_error_args, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "divide (zero error args 1)";
  Exception_reset();
  res = divide(zero_error_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;



  // Arguments: (0, int)
  // Expectation: None of them should raise exception.
  testname = "add (zero error args 2)";
  Exception_reset();
  res = add(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "subtract (zero error args 2)";
  Exception_reset();
  res = subtract(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "multiply (zero error args 2)";
  Exception_reset();
  res = multiply(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;

  testname = "divide (zero error args 2)";
  Exception_reset();
  res = divide(zero_error_args_swapped, NULL);
  if (Exception_check()) FAIL("raised exception");
  PASS;



  // Arguments: (int, int, int)
  // Expectation: All should raise exception.
  testname = "add (too many args 2)";
  Exception_reset();
  res = add(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "subtract (too many args 2)";
  Exception_reset();
  res = subtract(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "multiply (too many args 2)";
  Exception_reset();
  res = multiply(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  PASS;

  testname = "divide (too many args 2)";
  Exception_reset();
  res = divide(too_many_args, NULL);
  if (!Exception_check()) FAIL("didn't raise exception");
  Exception_reset();
  PASS;

  return 0;
}

extern int DEBUG_MODE;
int main(void)
{
  srand(0); // constant random seed
  int rc =0;
  //DEBUG_MODE = 1;
  

  RUN_TEST(builtins_integer_arithmetic_test);
  RUN_TEST(builtins_float_arithmetic_test);

  fprintf(stderr, FG_GREEN""BOLD"ALL TESTS PASSED."RESET"\n");

}
