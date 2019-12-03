#pragma once
#include <string>
#include <regex>
#include <iostream>
#include <list>
#include <memory>

#include "colour.hpp"

#define OP(O) LispAtom operator O(LispAtom obj) { \
\
  switch (this->get_type()) {\
    case LISPATOM_INT:\
      return LispAtom(this->value_int O obj.cast_to_int());\
    case LISPATOM_FLOAT:\
      return LispAtom(this->value_float O obj.cast_to_float());\
    case LISPATOM_STRING:\
      throw "Numerical operation O not defined for string.";\
  }\
  throw "Unknown type encountered!.";\
}

enum LISPATOM_TYPE{
  LISPATOM_INT,
  LISPATOM_FLOAT,
  LISPATOM_STRING
};



class Printable {
  public:
    Printable(){};
    virtual std::string repr(){ throw std::runtime_error("REPR NOT IMPLEMENTED IN PRINTABLE OBJECT."); }
    void print(){ this->_print(); std::cout << std::endl; };
    virtual void _print() { std::cout << this->repr() << " "; }
};

class LispBuiltin; 
typedef std::shared_ptr<LispBuiltin> LispBuiltin_ptr;

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


class LispAtom : virtual public Printable {
  private:
    long value_int;
    double value_float;
    std::string value_string;
    int type;

  public:
    LispAtom(long value) { 
      this->type = LISPATOM_INT;
      this->value_int = value; 
    }
    LispAtom(bool value) {
      this->type = LISPATOM_INT;
      this->value_int = (long)value; 
    }
    LispAtom(double value) { 
      this->type = LISPATOM_FLOAT;
      this->value_float = value; 
    }
    LispAtom(std::string value) { 
      this->type = LISPATOM_STRING;
      this->value_string = value; 
    }

    int get_type() { return this->type; }
    std::string repr();

    LispAtom operator +(LispAtom obj) {

      switch (this->get_type()) {

        case LISPATOM_INT:
          return LispAtom(this->value_int + obj.cast_to_int());

        case LISPATOM_FLOAT:
          return LispAtom(this->value_float + obj.cast_to_float());

        case LISPATOM_STRING:
          return LispAtom(this->value_string + obj.cast_to_string() );
      }

      throw "Unknown type encountered";
    }
   
   
    OP(-); OP(*); OP(/);
    OP(>); OP(>=); OP(<); OP(<=); OP(==);

    long cast_to_int();
    double cast_to_float();
    std::string cast_to_string();
};
typedef std::shared_ptr<LispAtom> LispAtom_ptr;








class LispSymbol : virtual public Printable{
  private:
    std::string name;
  public:
    LispSymbol() {};
    LispSymbol(std::string name){ this->name = name; }
    std::string get_name(){ return this->name; }
    std::string repr() { return this->name; }
};















enum LISPOBJECT_TYPE {
  LISPOBJECT_ATOM,
  LISPOBJECT_SYMBOL,
  LISPOBJECT_LIST
};


class LispObject : virtual public Printable {
  private:
    LispAtom_ptr value_atom;
    LispList_ptr value_list;
    LispSymbol_ptr value_symbol;
    int type;

  public:
    LispObject() {
      this->type = -1;
    };

    std::string repr();
    std::string repr_type();

    int get_type(){ return this->type; }
    LispObject_ptr eval(LispEnvironment_ptr env);

    template<typename T>
    LispObject(T atom_val) { this->value_atom = std::make_shared<LispAtom>(LispAtom(atom_val)); this->type = LISPOBJECT_ATOM; }
    LispObject(LispAtom_ptr atom) { this->value_atom = atom; this->type = LISPOBJECT_ATOM; }
    LispObject(LispList_ptr list) { this->value_list = list; this->type = LISPOBJECT_LIST; }
    LispObject(LispSymbol_ptr symbol) { this->value_symbol = symbol; this->type = LISPOBJECT_SYMBOL; }

    void set_value(LispAtom_ptr atom) {this->value_atom = atom;}
    void set_value(LispList_ptr list) {this->value_list = list; }
    void set_value(LispSymbol_ptr symbol) {this->value_symbol = symbol; }

    LispAtom_ptr get_value_atom() { return this->value_atom; }
    LispList_ptr get_value_list() { return this->value_list; }
    LispSymbol_ptr get_value_symbol() { return this->value_symbol; }

};







class LispList : virtual public Printable {
  private:
    std::list<LispObject_ptr> obj_list;

  public:
    LispList(){};

    LispList(std::list<LispObject_ptr>::iterator from, std::list<LispObject_ptr>::iterator to){ 
      this->obj_list = std::list<LispObject_ptr>(from, to);
    }

    std::list<LispObject_ptr>::iterator end() { return this->obj_list.end(); }
    std::list<LispObject_ptr>::iterator begin() { return this->obj_list.begin(); }
    LispObject_ptr first() { return this->obj_list.front(); }
    LispObject_ptr next(bool restart = false) {
      if (restart) {
        this->it = this->begin();
      }
      else {
        this->it++;
      }

      return (*this->it);
    }

    LispList_ptr rest() { 
      return std::make_shared<LispList>(LispList(++this->begin(), this->end())); 
    }

    int count() { return this->obj_list.size(); }
    void append(LispObject_ptr next_value) { this->obj_list.push_back(next_value); }
    std::string repr();

    LispObject_ptr eval_each(LispEnvironment_ptr env);
};






class LispToken;
typedef std::shared_ptr<LispToken> LispToken_ptr;
class LispToken {
  private:
    std::string token;

  public:
    LispToken_ptr next;

    LispToken(std::string token);
    std::string get_token() {return this->token; }
    int compare(std::string s) { return this->token.compare(s); }
    void print();
};












class LispParser {
  private:
    std::regex string_is_int_re;
    std::regex string_is_float_re;
    std::regex string_is_string_re;
    LispObject_ptr new_object_guess_type(LispToken_ptr t);
  public:
    LispParser() {
      this->string_is_int_re = std::regex("^-?\\d+$");
      this->string_is_float_re = std::regex("^-?\\d+(\\.|e\\+|e-|e)\\d+$");
      this->string_is_string_re = std::regex("^\".*\"$");
    }

    int string_is_int(std::string s) { return std::regex_match(s, this->string_is_int_re); }
    int string_is_float(std::string s) { return std::regex_match(s, this->string_is_float_re); }
    int string_is_string(std::string s) { return std::regex_match(s, this->string_is_string_re); }

    LispList_ptr parse(LispToken_ptr tokens);
    LispList_ptr parse_string(char *char_arr);
    LispList_ptr parse_string(std::string s);
    LispList_ptr parse_file(const char* path);
    LispList_ptr parse_file(std::string path);

    int count_parens(char *s);
    int count_parens(std::string s);
};



enum LISPENV_RET { LISPENV_OBJ, LISPENV_BFUNC, LISPENV_LFUNC };
class LispEnvironment {
  private:
    std::map<std::string, LispObject_ptr> objects_map;
    std::map<std::string, LispBuiltin_ptr> builtin_functions_map;
    std::map<std::string, LispFunction_ptr> lisp_functions_map;
    LispEnvironment_ptr parent;

  public:
    LispEnvironment();
    LispEnvironment(LispEnvironment_ptr parent) { this->parent = parent; }

    void add(std::string name, LispObject_ptr obj) { this->objects_map.insert_or_assign(name, obj); }
    void add(std::string name, LispBuiltin_ptr val){ this->builtin_functions_map.insert_or_assign(name, val); }
    void add(std::string name, LispFunction_ptr val){ this->lisp_functions_map.insert_or_assign(name, val); }
    void add_something(std::string name, LispObject_ptr obj, LispBuiltin_ptr bfunc, LispFunction_ptr lfunc);

    LispObject_ptr get_object(std::string name) { return this->objects_map[name]; }
    LispBuiltin_ptr get_builtin(std::string name) { return this->builtin_functions_map[name]; }
    int get(std::string name, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);

};




// 
typedef LispObject_ptr (LispCppFunc)(LispList_ptr arg, LispEnvironment_ptr env);
class LispBuiltin {
  private:
    LispCppFunc *func;
  public:
    LispBuiltin(LispCppFunc *func) { this->func = func; }
    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) { return this->func(arg, env); }
};





class LispFunction : virtual public Printable {
  private:
    std::vector<std::string> arg_names;
    LispObject_ptr body;
  public:
    LispFunction(){};
    LispFunction(LispList_ptr arglist, LispObject_ptr body)
    {
      this->body = body;
      for (auto iter = arglist->begin(); iter != arglist->end(); ++iter) {
        this->add_arg( (*iter)->get_value_symbol()->get_name() );
      }
    };
    void add_arg(std::string arg){ this->arg_names.push_back(arg); }
    void set_body(LispObject_ptr body){ this->body = body; }
    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env);
    std::string repr() { return this->body->repr(); }
};




struct environment_table_row {
  const char *name;
  const char *alias;

  const char *doc;

  LispObject_ptr obj;
  LispBuiltin_ptr bfunc;
  LispFunction_ptr lfunc;
};
