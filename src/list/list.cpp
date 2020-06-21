#include "../types.hpp"
#include "list.hpp"
#include "../object/object.hpp"


std::string LispList::repr()
{
  std::stringstream ss;
  ss << "( ";
  for (auto const& it : this->obj_list) {
    ss << it->repr() << " ";
  }
  ss << ")";
  return ss.str();
}


std::string LispList::str()
{
  std::stringstream ss;
  ss << "( ";
  for (auto const& it : this->obj_list) {
    ss << it->str() << " ";
  }
  ss << ")";
  return ss.str();
}


bool LispList::eq(const LispList_ptr &other) const
{

  if (this->count() != other->count()) {
    return false;
  }

  auto itthis = this->begin(), itother = other->begin();
  for (; (itthis != this->end()) and (itother != other->end()); itthis++,itother++) {
    auto objthis = (*itthis), objother = (*itother);
    if (not objthis->eq(objother))
      return false;
  }

  return true;
}
