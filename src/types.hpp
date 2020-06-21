#pragma once
#include <string>
#include <fstream>
#include <ios>
#include <regex>
#include <iostream>
#include <list>
#include <memory>

#include "colour.hpp"
#include "debug.hpp"
#include "exception.hpp"
#include "formatter.hpp"
#include "object/object_types.hpp"
#include "atom/atom_types.hpp"
#include "env/env_types.hpp"
#include "traceable/traceable_types.hpp"


class LispBuiltin; 
//typedef std::shared_ptr<LispBuiltin> LispBuiltin_ptr;
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













class LispParser {
  private:
    std::regex string_is_int_re;
    std::regex string_is_float_re;
    std::regex string_is_string_re;
    LispObject_ptr new_object_guess_type(LispToken_ptr t);
    std::list<std::string> module_paths;
    std::string search_module(std::string name);
  public:
    LispParser();

    int string_is_int(std::string s);
    int string_is_float(std::string s);
    int string_is_string(std::string s);

    LispList_ptr parse_string(char *char_arr, TraceSource proto_source);
    LispList_ptr parse_string(std::string s, TraceSource proto_source);
    LispList_ptr parse_module(const char* path);
    LispList_ptr parse_module(std::string path);

    LispList_ptr parse(LispToken_ptr tokens);
    LispList_ptr parse_file(const char* path);
    LispList_ptr parse_file(std::string path);

    int count_parens(char *s);
    int count_parens(std::string s);
};






class Executable {
  private:
    bool _is_macro;
  public:
    Executable() { this->_is_macro = false; }
    ~Executable() { }
    void set_macro() { this->_is_macro = true; }
    bool is_macro() const { return this->_is_macro; }
    virtual LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const =0;
};






class LispBuiltin : public virtual Printable, public virtual Executable, public virtual Documented {
};





class LispFunction : virtual public Printable, virtual public Executable, virtual public Documented {
  private:
    std::vector<std::string> arg_names;
    std::vector<LispObject_ptr> arg_default_values;
    LispList_ptr body;
    std::string name;
    std::string doc;
    void add_arg(std::string arg){ this->arg_names.push_back(arg); }

    // TODO: remove this heinous crime
    mutable LispObject_ptr result;

  public:
    LispFunction() : Executable() {};
    LispFunction(LispList_ptr arglist, LispList_ptr defvalue_list, LispList_ptr body, std::string name, std::string doc, bool is_macro=false)
      : Executable(), body(body), name(name), doc(doc)
    {
      this->body = body;
      this->name = name;
      this->doc = doc;

      for (auto iter = arglist->begin(); iter != arglist->end(); ++iter) {
        this->add_arg( (*iter)->get_value_symbol()->get_name() );
      }

      for (auto iter = defvalue_list->begin(); iter != defvalue_list->end(); ++iter) {
        this->arg_default_values.push_back(*iter);
      }

      if (is_macro)
        this->set_macro();
    };
    LispFunction(LispList_ptr arglist, LispList_ptr body, std::string name, std::string doc, bool is_macro=false)
      : LispFunction(arglist, std::make_shared<LispList>(), body, name, doc, is_macro)
    {
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const;
    const std::string repr() const { return this->body->repr(); }
    const std::string str() const {return this->body->str(); }
    const std::string get_doc() const override { return this->doc; }

    LispObject_ptr get_result() { return this->result ? this->result : std::make_shared<LispObject>(std::make_shared<LispList>()); }
};




typedef struct environment_table_row {
  const char *name;
  const char *alias;
  LispObject_ptr obj;
  LispBuiltin_ptr bfunc;
  LispFunction_ptr lfunc;
} LispEnvironmentRow;
