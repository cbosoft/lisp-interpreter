#include <memory>
#include "types.hpp"
#include "pointer.hpp"
#include "exception.hpp"

template <typename T>
std::shared_ptr<T> make_ptr(T obj)
{
  std::shared_ptr<T> rv = std::make_shared<T>(obj);
  if (rv == NULL)
    throw Exception("MEMORY ERROR", "RUN OUT OF THE MEMS");

  return rv;
}

template std::shared_ptr<LispObject> make_ptr(LispObject);
// template std::shared_ptr<LispBuiltin> make_ptr(LispBuiltin);
template std::shared_ptr<LispFunction> make_ptr(LispFunction);
template std::shared_ptr<LispAtom> make_ptr(LispAtom);
template std::shared_ptr<LispEnvironment> make_ptr(LispEnvironment);
template std::shared_ptr<LispList> make_ptr(LispList);
