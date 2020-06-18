#include "types.hpp"

LispObject_ptr t = std::make_shared<LispObject>(true);
LispObject_ptr nil = std::make_shared<LispObject>(std::make_shared<LispList>());
