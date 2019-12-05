#pragma once
#include <string>
#include <regex>
#include <iostream>
#include <list>
#include <memory>

#include "colour.hpp"
#include "debug.hpp"
#include "exception.hpp"


enum LISPATOM_TYPE{
  LISPATOM_INT,
  LISPATOM_FLOAT,
  LISPATOM_STRING
};


class Printable {
  public:
    Printable(){};
    virtual std::string repr(){ throw NotImplementedError("In Printable::repr(): virtual function not implemented: intended to be used by sub class."); }
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

class LispAtom;
typedef std::shared_ptr<LispAtom> LispAtom_ptr;

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
    std::string repr_type();
    long get_value_int(){ return this->value_int; }
    double get_value_float(){ return this->value_float; }
    std::string get_value_string(){ return this->value_string; }

    bool is_truthy();

    LispAtom_ptr add(LispAtom_ptr obj);
    LispAtom_ptr subtract(LispAtom_ptr obj);
    LispAtom_ptr multiply(LispAtom_ptr obj);
    LispAtom_ptr divide(LispAtom_ptr obj);
    LispAtom_ptr power(LispAtom_ptr obj);
    LispAtom_ptr modulo(LispAtom_ptr obj);

    LispAtom_ptr gt(LispAtom_ptr obj);
    LispAtom_ptr ge(LispAtom_ptr obj);
    LispAtom_ptr lt(LispAtom_ptr obj);
    LispAtom_ptr le(LispAtom_ptr obj);
    LispAtom_ptr eq(LispAtom_ptr obj);

    long cast_to_int();
    double cast_to_float();
    std::string cast_to_string();
};








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
    LispObject(const LispObject& o) {
      this->type = o.type;
      this->value_symbol = o.value_symbol;
      this->value_atom = o.value_atom;
      this->value_list = o.value_list;
    }

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

    bool is_truthy();

};







class LispList : virtual public Printable {
  private:
    std::list<LispObject_ptr> obj_list;
    std::list<LispObject_ptr>::iterator it;

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
    LispParser();

    int string_is_int(std::string s);
    int string_is_float(std::string s);
    int string_is_string(std::string s);

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

    void add(std::string name, LispObject_ptr obj);
    void add(std::string name, LispBuiltin_ptr val);
    void add(std::string name, LispFunction_ptr val);
    void add_something(std::string name, LispObject_ptr obj, LispBuiltin_ptr bfunc, LispFunction_ptr lfunc);

    int get(std::string name, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);

};




class Executable {
  private:
    bool _is_macro;
  public:
    Executable() { this->_is_macro = false; }
    ~Executable() { }
    void set_macro() { this->_is_macro = true; }
    bool is_macro() { return this->_is_macro; }
    virtual LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) =0;
};




// 
typedef LispObject_ptr (LispCppFunc)(LispList_ptr arg, LispEnvironment_ptr env);
class LispBuiltin : virtual public Printable, virtual public Executable {
  private:
    LispCppFunc *func;
  public:
    LispBuiltin() : Executable () {}
    LispBuiltin(LispCppFunc *func) : Executable() { 
      debug_message("Constructing new lisp function.");
      this->func = func; 
    }
    LispBuiltin(LispCppFunc *func, bool is_macro) : Executable() { 
      this->func = func; 
      if (is_macro) {
        debug_message("Constructing new lisp mecro.");
        this->set_macro(); 
      }
      else {
        debug_message("Constructing new lisp function.");
      }
    }
    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) { return this->func(arg, env); }
    std::string repr();
};





class LispFunction : virtual public Printable, virtual public Executable {
  private:
    std::vector<std::string> arg_names;
    LispObject_ptr body;
  public:
    LispFunction() : Executable() {};
    LispFunction(LispList_ptr arglist, LispObject_ptr body)
      : Executable()
    {
      this->body = body;
      for (auto iter = arglist->begin(); iter != arglist->end(); ++iter) {
        this->add_arg( (*iter)->get_value_symbol()->get_name() );
      }
    };
    LispFunction(LispList_ptr arglist, LispObject_ptr body, bool is_macro)
      : Executable()
    {
      this->body = body;
      for (auto iter = arglist->begin(); iter != arglist->end(); ++iter) {
        this->add_arg( (*iter)->get_value_symbol()->get_name() );
      }
      if (is_macro) this->set_macro();
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
