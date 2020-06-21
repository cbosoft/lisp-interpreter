#pragma once
#include <string>
#include <fstream>
#include <ios>
#include <regex>
#include <iostream>
#include <list>
#include <memory>

#include "object/object_types.hpp"
#include "atom/atom_types.hpp"
#include "env/env_types.hpp"
#include "traceable/traceable_types.hpp"


class LispBuiltin;
typedef const LispBuiltin * LispBuiltin_ptr;

class LispFunction;
typedef std::shared_ptr<LispFunction> LispFunction_ptr;

class LispEnvironment;
typedef std::shared_ptr<LispEnvironment> LispEnvironment_ptr;

class LispSymbol;
typedef std::shared_ptr<LispSymbol> LispSymbol_ptr;

class LispList;
typedef std::shared_ptr<LispList> LispList_ptr;

class LispObject;
typedef std::shared_ptr<LispObject> LispObject_ptr;

class LispAtom;
typedef std::shared_ptr<LispAtom> LispAtom_ptr;

class LispToken;
typedef std::shared_ptr<LispToken> LispToken_ptr;

typedef struct environment_table_row {
  const char *name;
  const char *alias;
  LispObject_ptr obj;
  LispBuiltin_ptr bfunc;
  LispFunction_ptr lfunc;
} LispEnvironmentRow;
