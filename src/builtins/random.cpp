#include <random>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../builtins.hpp"


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
  
  return std::make_shared<LispObject>(LispObject( float_dist(randgen) ));
}

#undef FUNC
#define FUNC "randint"
// (randint) -> uniformly distributed pseudo-random integer in range (0, RAND_MAX)
LispObject_ptr randint(LispList_ptr arg, LispEnvironment_ptr env)
{
  debug_message(Formatter() << "builtin function " << FUNC);
  (void) env;

  int nargs = arg->count();
  if (nargs != 0) throw ArgumentError(Formatter() << "In " << FUNC << ": Wrong number of arguments supplied. Got " << nargs << ", expected 0.");
  
  return std::make_shared<LispObject>(LispObject( integer_dist(randgen) ));
}
