#pragma once
#include <string>
#include <regex>
#include <iostream>

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
      std::cout << this->repr() << std::endl;
    }
};








class LispSymbol {
  private:
    std::string name;
  public:
    LispSymbol() {};
    LispSymbol(std::string name){ this->name = name; }
    void print() {
      std::cout << " " << this->name << " " << std::endl;
    }
};















enum LISPOBJECT_TYPE {
  LISPOBJECT_ATOM,
  LISPOBJECT_SYMBOL,
  LISPOBJECT_LIST
};


class LispListElement;
class LispObject {
  private:
    LispAtom *value_atom;
    LispListElement *value_list;
    LispSymbol *value_symbol;
    int type;

  public:
    LispObject() {
      this->value_atom = NULL;
      this->value_list = NULL;
      this->value_symbol = NULL;
      this->type = -1;
    };

    LispObject(LispAtom *atom) { this->value_atom = atom; this->type = LISPOBJECT_ATOM; }
    LispObject(LispListElement *list) { this->value_list = list; this->type = LISPOBJECT_LIST; }
    LispObject(LispSymbol *symbol) { this->value_symbol = symbol; this->type = LISPOBJECT_SYMBOL; }

    void set_value(LispAtom *atom) {this->value_atom = atom;}
    void set_value(LispListElement *list) {this->value_list = list; }
    void set_value(LispSymbol *symbol) {this->value_symbol = symbol; }

    LispAtom *get_value_atom() { return this->value_atom; }
    LispListElement *get_value_list() { return this->value_list; }
    LispSymbol *get_value_symbol() { return this->value_symbol; }

    void print();
    // TODO comparison
};







class LispListElement {
  private:
    LispListElement *next;
    LispObject *value;

  public:
    LispListElement() {
      this->value = NULL;
      this->next = NULL;
    }
    void set_value(LispObject *value) { this->value = value; }
    LispObject *get_value(LispObject *value) { return this->value; }

    void append(LispObject *next_value) { 
      LispListElement *iter = this;
      while (iter->next != NULL) iter = iter->next;
      iter->next = new LispListElement();
      iter->value = next_value;
    }

    void print() {
      
      this->value->print();

      LispListElement *iter = this->next;
      while (iter->next != NULL) {
        iter->print();
        iter = iter->next;
      }

    }
};













inline void LispObject::print() {
  //if (this == 0) throw "can't print null obj";

  std::cout << "PRINTING " << this << std::endl;

  switch (this->type) {
    case LISPOBJECT_ATOM:
      std::cout << "PRINTING ATOM\n";
      this->value_atom->print();
      break;

    case LISPOBJECT_SYMBOL:
      std::cout << "PRINTING SYMBOL\n";
      this->value_symbol->print();
      break;

    case LISPOBJECT_LIST:
      std::cout << "PRINTING LIST\n";
      std::cout << "(";
      this->value_list->print();
      std::cout << ")";
      break;
  }

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

    LispListElement *parse(LispToken *tokens);
};
