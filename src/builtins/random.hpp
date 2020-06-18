#pragma once
#include <random>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"


static std::random_device rd;
static std::mt19937 randgen(rd());
std::uniform_int_distribution<> integer_dist(0, RAND_MAX);
std::uniform_real_distribution<> float_dist(0, 1);


class LispFunc_random : public virtual LispBuiltin {
  private:
    inline static const std::string name = "random";
    inline static const std::string doc = 
      "(random)\n\
      Returns a uniformly distributed pseudo-random float in the range (0 1).\n\
      Uses the Merseinne-Twister (19937) algorithm.";

  public:
    LispFunc_random() {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 0, this->name, "");
      return std::make_shared<LispObject>(float_dist(randgen));
    }
};


// (randint) -> uniformly distributed pseudo-random integer in range (0, RAND_MAX)
class LispFunc_randint : public virtual LispBuiltin {
  private:
    inline static const std::string name = "randint";
    inline static const std::string doc = 
      "(randint)\n\
      Returns a uniformly distributed pseudo random integer in the range (0, RAND_MAX)\n\
      Uses the Merseinne-Twister (19937) algorithm.";

  public:
    LispFunc_randint()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin)";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check(arg, 0, this->name, "");
      return std::make_shared<LispObject>( integer_dist(randgen) );
    }
};
