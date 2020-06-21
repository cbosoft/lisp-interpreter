#pragma once

#include "../printable/printable.hpp"
#include "../executable/executable.hpp"
#include "../documented/documented.hpp"


class LispBuiltin :
  public virtual Printable,
  public virtual Executable,
  public virtual Documented
{
  //
};
