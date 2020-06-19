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


enum LispAtom_Type {
  LISPATOM_INT,
  LISPATOM_FLOAT,
  LISPATOM_STRING
};


class Printable {
  public:
    Printable(){};
    virtual const std::string repr() const{ throw NotImplementedError("In Printable::repr(): virtual function not implemented: intended to be used by sub class."); }
    virtual const std::string str() const{ throw NotImplementedError("In Printable::str(): virtual function not implemented: intended to be used by sub class."); }
    void print(){ this->_print(); std::cout << std::endl; };
    virtual void _print() { std::cout << this->repr() << " "; }
};

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

class LispAtom : virtual public Printable {
  private:
    long value_int;
    double value_float;
    std::string value_string;
    LispAtom_Type type;

  public:
    LispAtom(long value) { 
      this->type = LISPATOM_INT;
      this->value_int = value; 
    }
    LispAtom(bool value) : LispAtom((long)value) {};
    LispAtom(int value) : LispAtom((long)value) {};

    LispAtom(double value) { 
      this->type = LISPATOM_FLOAT;
      this->value_float = value; 
    }
    LispAtom(float value) : LispAtom((double)value) {};

    LispAtom(std::string value);
    LispAtom(const char* value) : LispAtom(std::string(value)) {};

    LispAtom_Type get_type() { return this->type; }
    const std::string str() const;
    const std::string repr() const;
    std::string repr_type();
    static std::string repr_type(LispAtom_Type& type);
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
    const std::string repr() const override { return this->name; }
    const std::string str() const override {return this->name; }
};







enum TraceSource_Type {
  TRACESOURCE_REPL,
  TRACESOURCE_ARGUMENT,
  TRACESOURCE_FILE
};

typedef struct _TraceSource {
  TraceSource_Type type;
  std::string path_or_commands;
  int row;
  int column;
  int token_length;
} TraceSource;
typedef std::shared_ptr<TraceSource> TraceSource_ptr;

class Traceable {
  private:
    TraceSource_ptr source;
    const std::string get_file_trace() const;
  public:
    Traceable() { }

    void set_source(TraceSource_ptr source)
    {
      this->source = source;
    }

    TraceSource_ptr get_source()
    {
      return this->source;
    }

    const std::string repr_source();

    template<typename T>
    void inherits_from(std::shared_ptr<T> obj) {
      this->source = obj->get_source();
      debug_message(Formatter() << "inherited: " << this->repr_source());
    }

    //void inherits_from(std::shared_ptr<LispObject> obj);
};







class Documented {
  public:
    virtual const std::string get_doc() const { return "Documentation for this object not available."; }
};




enum LispObject_Type {
  LISPOBJECT_ATOM,
  LISPOBJECT_SYMBOL,
  LISPOBJECT_LIST
};


class LispObject : virtual public Printable, public Traceable, virtual public Documented {
  private:
    LispAtom_ptr value_atom;
    LispList_ptr value_list;
    LispSymbol_ptr value_symbol;
    LispObject_Type type;
    std::string doc;

  public:
    LispObject();

    const std::string str() const override;
    const std::string repr() const override;
    std::string repr_type() const;
    std::string static repr_type(LispObject_Type type);

    LispObject_Type get_type() const { return this->type; }
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
    const LispAtom_ptr get_value_atom() const { return this->value_atom; }
    LispList_ptr get_value_list() { return this->value_list; }
    const LispList_ptr get_value_list() const { return this->value_list; }
    LispSymbol_ptr get_value_symbol() { return this->value_symbol; }
    const LispSymbol_ptr get_value_symbol() const { return this->value_symbol; }

    bool is_truthy(LispEnvironment_ptr env=nullptr);
    bool eq(const LispObject_ptr &other) const;

    const std::string get_doc() const override { return (this->doc.size() ? this->doc : this->repr()); }
    void set_doc(std::string s) { this->doc = s; }

};







class LispList : virtual public Printable {
  private:
    std::list<LispObject_ptr> obj_list;
    std::list<LispObject_ptr>::const_iterator it;

  public:
    LispList(){};

    LispList(std::list<LispObject_ptr>::const_iterator from, std::list<LispObject_ptr>::const_iterator to){ 
      this->obj_list = std::list<LispObject_ptr>(from, to);
    }

    std::list<LispObject_ptr>::const_iterator end() const { return this->obj_list.end(); }
    std::list<LispObject_ptr>::const_iterator begin() const { return this->obj_list.begin(); }
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

    LispList_ptr rest(int n = 1) { 
      auto it = this->begin();
      for (int i = 0; i < n; i++, ++it);

      if (it == this->end())
        return std::make_shared<LispList>();

      return std::make_shared<LispList>(LispList(it, this->end())); 
    }

    int count() const { return this->obj_list.size(); }
    void append(LispObject_ptr next_value) { this->obj_list.push_back(next_value); }
    std::string repr();
    std::string str();
    bool eq(const LispList_ptr &other) const
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

    LispObject_ptr eval_each(LispEnvironment_ptr env) const;
};






class LispToken;
typedef std::shared_ptr<LispToken> LispToken_ptr;
class LispToken : public Traceable {
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



enum LispEnvironment_Type { LISPENV_OBJ, LISPENV_BFUNC, LISPENV_LFUNC, LISPENV_NOTFOUND = -1 };
class LispEnvironment {
  private:
    std::map<std::string, LispObject_ptr> objects_map;
    std::map<std::string, LispBuiltin_ptr> builtin_functions_map;
    std::map<std::string, LispFunction_ptr> lisp_functions_map;
    bool readonly = false;
    LispEnvironment_ptr parent;

  public:
    LispEnvironment();
    LispEnvironment(LispEnvironment_ptr parent, bool readonly = false) 
    { 
      this->parent = parent; 
      this->readonly = readonly; 
    }

    void add(std::string name, LispObject_ptr obj);
    void add(std::string name, LispBuiltin_ptr val);
    void add(std::string name, LispFunction_ptr val);
    void add_something(std::string name, LispObject_ptr obj, LispBuiltin_ptr bfunc, LispFunction_ptr lfunc);

    LispEnvironment_Type get(LispSymbol_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);
    LispEnvironment_Type get(LispObject_ptr sym, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);
    LispEnvironment_Type get(std::string name, LispObject_ptr *obj, LispBuiltin_ptr *bf, LispFunction_ptr *lf);
    LispEnvironment_ptr get_parent() const {
      return this->parent;
    }

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
