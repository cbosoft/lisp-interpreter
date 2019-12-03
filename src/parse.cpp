#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <ios>

#include "types.hpp"
#include "debug.hpp"
#include "tokenise.hpp"
#include "formatter.hpp"






// Create new object, guessing type
LispObject_ptr LispParser::new_object_guess_type(LispToken_ptr t) {
  std::string s = t->get_token();

  debug_message(Formatter() << "GUESSING " << s);
  
  if (s.compare("(") == 0) {
    debug_message(Formatter() << "GUESSING "<< s << "is LIST");
    LispList_ptr newlist = std::make_shared<LispList>(LispList());
    return std::make_shared<LispObject>(LispObject(newlist));
  }

  if (this->string_is_int(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is INT");
    LispAtom_ptr newatom = std::make_shared<LispAtom>(LispAtom( atol(s.c_str()) ));
    return std::make_shared<LispObject>(LispObject(newatom));
  }

  if (this->string_is_float(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is FLOAT");
    LispAtom_ptr newatom = std::make_shared<LispAtom>(LispAtom( atof(s.c_str()) ));
    return std::make_shared<LispObject>(LispObject(newatom));
  }


  if (this->string_is_string(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is STRING");
    std::string body = s.substr(1, s.length()-1);
    LispAtom_ptr newatom = std::make_shared<LispAtom>(LispAtom(body));
    return std::make_shared<LispObject>(LispObject(newatom));
  }

  debug_message(Formatter() << "GUESSING " << s << " is SYMBOL");
  LispSymbol_ptr newsymbol = std::make_shared<LispSymbol>(LispSymbol(s));
  return std::make_shared<LispObject>(LispObject(newsymbol));
}




// Parse a list of tokens into AST/lisp data
LispList_ptr LispParser::parse(LispToken_ptr tokens)
{
  LispList_ptr root = std::make_shared<LispList>(LispList());
  LispObject_ptr new_obj;
  std::stack<LispList_ptr> open_lists;
  open_lists.push(root);

  debug_message("in parse");

  for (LispToken_ptr token = tokens; token != NULL; token = token->next) {
    debug_message(Formatter() << "token: " << token->get_token());

    if (token->compare("(") == 0) {

      debug_message("opening new list");
      LispList_ptr newlist = std::make_shared<LispList>(LispList());
      new_obj = std::make_shared<LispObject>(LispObject(newlist));
      open_lists.top()->append(new_obj);
      open_lists.push(newlist);
      debug_message("after opening new list");

    }
    else if (token->compare(")") == 0) {

      debug_message("closing list");
      open_lists.pop();

    }
    else {

      debug_message("guess");
      new_obj = this->new_object_guess_type(token);

      debug_message(Formatter() << "new object " << new_obj->repr());
      open_lists.top()->append(new_obj);

    }

  }

  return root;
}




// parse
LispList_ptr LispParser::parse_string(char *char_arr){ return this->parse_string(std::string(char_arr)); }
LispList_ptr LispParser::parse_string(std::string s)
{

  LispToken_ptr tokens = tokenise(s);

  debug_message("tokens:");
  for (LispToken_ptr i = tokens; i != NULL; i = i->next) {
    debug_message(Formatter() << "| " << i->get_token());
  }

  return this->parse(tokens);
}




// parse file
LispList_ptr LispParser::parse_file(const char *path){ return this->parse_file(std::string(path)); }
LispList_ptr LispParser::parse_file(std::string path)
{
  std::ifstream ifs(path);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  return this->parse_string(buffer.str());
}


int LispParser::count_parens(char * s) {return this->count_parens(std::string(s)); }
int LispParser::count_parens(std::string s)
{
  int nopen = 0;
  for (auto &c : s) {
    if (c == '(')
      nopen ++;
    else if (c == ')')
      nopen --;
  }
  return nopen;
}