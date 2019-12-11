#include <random>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../pointer.hpp"


static std::random_device rd;
static std::mt19937 randgen(rd());
std::uniform_int_distribution<> integer_dist(0, RAND_MAX);
std::uniform_real_distribution<> float_dist(0, 1);

#define FUNC "random"
// (random) -> uniformly distributed pseudo-random float in range (0, 1)
LispObject_ptr random(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 0) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 0.");
  
  return make_ptr(LispObject( float_dist(randgen) ));
}

LispEnvironmentRow random_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&random, "(random)", false)),
  .lfunc = NULL
};

#undef FUNC
#define FUNC "randint"
// (randint) -> uniformly distributed pseudo-random integer in range (0, RAND_MAX)
LispObject_ptr randint(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 0) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 0.");
  
  return make_ptr(LispObject( integer_dist(randgen) ));
}

LispEnvironmentRow randint_row = {
  .name = FUNC,
  .alias = NULL,
  .obj = NULL,
  .bfunc = make_ptr(LispBuiltin(&randint, "(randint)", false)),
  .lfunc = NULL
};
