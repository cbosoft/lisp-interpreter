#include "../util/debug.hpp"
#include "env.hpp"


extern struct environment_table_row builtins[];
LispEnvironment::LispEnvironment()
{
  this->parent = NULL;
  int i = 0;
  struct environment_table_row row_i = builtins[0];
  for (i = 0, row_i = builtins[i]; row_i.name != NULL; i++, row_i = builtins[i]) {
    debug_message(Formatter() << row_i.name << row_i.obj << row_i.bfunc << row_i.lfunc);
    this->add_something(row_i.name, row_i.obj, row_i.bfunc, row_i.lfunc);
    if (row_i.alias != NULL) this->add_something(row_i.alias, row_i.obj, row_i.bfunc, row_i.lfunc);
  }
  this->readonly = false;
}


LispEnvironment::LispEnvironment(LispEnvironment_ptr parent, bool readonly)
  : readonly(readonly), parent(parent)
{
  // do nothing
}
