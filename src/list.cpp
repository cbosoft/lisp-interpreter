#include "types.hpp"


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
