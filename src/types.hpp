#pragma once
#include <string>
#include <regex>
#include <iostream>
#include <list>

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

class LispAtom {
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
    void print() {
      std::cout << " " << this->repr() << " ";
    }
};








class LispSymbol {
  private:
    std::string name;
  public:
    LispSymbol() {};
    LispSymbol(std::string name){ this->name = name; }
    std::string get_name(){ return this->name; }
    void print() {
      std::cout << " " << this->name << " ";
    }
};















enum LISPOBJECT_TYPE {
  LISPOBJECT_ATOM,
  LISPOBJECT_SYMBOL,
  LISPOBJECT_LIST
};


class LispEnvironment;
class LispList;
class LispObject {
  private:
    LispAtom *value_atom;
    LispList *value_list;
    LispSymbol *value_symbol;
    int type;

  public:
    LispObject() {
      this->value_atom = NULL;
      this->value_list = NULL;
      this->value_symbol = NULL;
      this->type = -1;
    };

    std::string repr() {
      switch (this->type) {
        case LISPOBJECT_ATOM:
          return this->value_atom->repr();

        case LISPOBJECT_LIST:
          return "(TODO)";

        case LISPOBJECT_SYMBOL:
          return this->value_symbol->get_name();
      }

      throw "Unknown type found!";
    }

    std::string repr_type() {
      switch (this->type) {
        case LISPOBJECT_ATOM:
          return "Atom";

        case LISPOBJECT_LIST:
          return "List";

        case LISPOBJECT_SYMBOL:
          return "Symbol";
      }

      throw "Unknown type found!";
    }

    int get_type(){ return this->type; }
    LispObject *eval(LispEnvironment *env);

    template<typename T>
    LispObject(T atom_val) { this->value_atom = new LispAtom(atom_val); this->type = LISPOBJECT_ATOM; }
    LispObject(LispAtom *atom) { this->value_atom = atom; this->type = LISPOBJECT_ATOM; }
    LispObject(LispList *list) { this->value_list = list; this->type = LISPOBJECT_LIST; }
    LispObject(LispSymbol *symbol) { this->value_symbol = symbol; this->type = LISPOBJECT_SYMBOL; }

    void set_value(LispAtom *atom) {this->value_atom = atom;}
    void set_value(LispList *list) {this->value_list = list; }
    void set_value(LispSymbol *symbol) {this->value_symbol = symbol; }

    LispAtom *get_value_atom() { return this->value_atom; }
    LispList *get_value_list() { return this->value_list; }
    LispSymbol *get_value_symbol() { return this->value_symbol; }

    void print();
    // TODO comparison
};







class LispList {
  private:
    std::list<LispObject *> obj_list;

  public:
    LispList(){};
    LispList(std::list<LispObject *>::iterator from, std::list<LispObject *>::iterator to){ this->obj_list = std::list<LispObject *>(from, to);}
    std::list<LispObject *>::iterator end() { return this->obj_list.begin(); }
    std::list<LispObject *>::iterator begin() { return this->obj_list.end(); }
    LispList *rest() { return new LispList(++this->begin(), this->end()); }

    int count() { return this->obj_list.size(); }
    void append(LispObject *next_value) { this->obj_list.push_back(next_value); }
    void print() { for (auto const& v : this->obj_list) v->print(); }

    LispObject *eval_each(LispEnvironment *env);
};













inline void LispObject::print() {
  //if (this == 0) throw "can't print null obj";

  switch (this->type) {
    case LISPOBJECT_ATOM:
      this->value_atom->print();
      break;

    case LISPOBJECT_SYMBOL:
      this->value_symbol->print();
      break;

    case LISPOBJECT_LIST:
      std::cout << "(";
      this->value_list->print();
      std::cout << ")";
      break;
  }
  std::cout << DIM << "(" << this << ")" << RESET;

}








class LispToken {
  private:
    std::string token;

  public:
    LispToken *next;

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
    LispObject *new_object_guess_type(LispToken *t);
  public:
    LispParser() {
      this->string_is_int_re = std::regex("^-?\\d+$");
      this->string_is_float_re = std::regex("^-?\\d+(\\.|e\\+|e-|e)\\d+$");
      this->string_is_string_re = std::regex("^\".*\"$");
    }

    int string_is_int(std::string s) { return std::regex_match(s, this->string_is_int_re); }
    int string_is_float(std::string s) { return std::regex_match(s, this->string_is_float_re); }
    int string_is_string(std::string s) { return std::regex_match(s, this->string_is_string_re); }

    LispList *parse(LispToken *tokens);
    LispList *parse_string(std::string s);
    LispList *parse_file(std::string path);
};



enum LISPENV_RET { LISPENV_OBJ, LISPENV_BFUNC, LISPENV_LFUNC };
class LispBuiltin; 
class LispFunction; 
class LispEnvironment {
  private:
    std::map<std::string, LispObject *> objects_map;
    std::map<std::string, LispBuiltin *> builtin_functions_map;
    std::map<std::string, LispFunction *> lisp_functions_map;
    LispEnvironment *parent;
  public:
    LispEnvironment() { this->parent = NULL; }
    LispEnvironment(LispEnvironment *parent) { this->parent = parent; }

    void add_variable(std::string name, LispObject *obj) { this->objects_map.insert_or_assign(name, obj); }
    void add_builtin(std::string name, LispBuiltin *val){ this->builtin_functions_map.insert_or_assign(name, val); }
    void add_lispfunc(std::string name, LispFunction *val){ this->lisp_functions_map.insert_or_assign(name, val); }

    LispObject *get_object(std::string name) { return this->objects_map[name]; }
    LispBuiltin *get_builtin(std::string name) { return this->builtin_functions_map[name]; }
    int get(std::string name, LispObject **obj, LispBuiltin **bf, LispFunction **lf) {
      auto obj_iter = this->objects_map.find(name);
      if (obj_iter != this->objects_map.end()) {
        (*obj) = obj_iter->second;
        return LISPENV_OBJ;
      }

      auto bf_iter = this->builtin_functions_map.find(name);
      if (bf_iter != this->builtin_functions_map.end()) {
        (*bf) = bf_iter->second;
        return LISPENV_BFUNC;
      }

      auto lf_iter = this->lisp_functions_map.find(name);
      if (lf_iter != this->lisp_functions_map.end()) {
        (*lf) = lf_iter->second;
        return LISPENV_LFUNC;
      }

      return -1;
    }
};




// 
typedef LispObject* (LispCppFunc)(LispList *arg, LispEnvironment *env);
class LispBuiltin {
  private:
    LispCppFunc *func;
  public:
    LispBuiltin(LispCppFunc *func) { this->func = func; }
    LispObject *eval(LispList *arg, LispEnvironment *env) { return this->func(arg, env); }
};





class LispFunction {
  private:
    std::vector<std::string> arg_names;
    LispObject *body;
  public:
    LispFunction(){};
    void add_arg(std::string arg){ this->arg_names.push_back(arg); }
    void set_body(LispObject *body){ this->body = body; }
    LispObject *eval(LispList *arg, LispEnvironment *env);
};
