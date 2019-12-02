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
LispObject *LispParser::new_object_guess_type(LispToken *t) {
  std::string s = t->get_token();

  debug_message(Formatter() << "GUESSING " << s);
  
  if (s.compare("(") == 0) {
    debug_message(Formatter() << "GUESSING "<< s << "is LIST");
    LispList *newlist = new LispList();
    return new LispObject(newlist);
  }

  if (this->string_is_int(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is INT");
    LispAtom *newatom = new LispAtom( atol(s.c_str()) );
    return new LispObject(newatom);
  }

  if (this->string_is_float(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is FLOAT");
    LispAtom *newatom = new LispAtom( atof(s.c_str()) );
    return new LispObject(newatom);
  }


  if (this->string_is_string(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is STRING");
    std::string body = s.substr(1, s.length()-1);
    LispAtom *newatom = new LispAtom(body);
    return new LispObject(newatom);
  }

  debug_message(Formatter() << "GUESSING " << s << " is SYMBOL");
  LispSymbol *newsymbol = new LispSymbol(s);
  return new LispObject(newsymbol);
}




// Parse a list of tokens into AST/lisp data
LispList *LispParser::parse(LispToken *tokens)
{
  LispList *root = new LispList();
  LispObject *new_obj = NULL;
  std::stack<LispList *> open_lists;
  open_lists.push(root);

  debug_message("in parse");

  for (LispToken *token = tokens; token != NULL; token = token->next) {
    debug_message(Formatter() << "token: " << token->get_token());

    if (token->compare("(") == 0) {

      debug_message("opening new list");
      LispList *newlist = new LispList();
      new_obj = new LispObject(newlist);
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
LispList *LispParser::parse_string(char *char_arr){ return this->parse_string(std::string(char_arr)); }
LispList *LispParser::parse_string(std::string s)
{

  LispToken *tokens = tokenise(s);

  debug_message("tokens:");
  for (LispToken *i = tokens; i != NULL; i = i->next) {
    debug_message(Formatter() << "| " << i->get_token());
  }

  return this->parse(tokens);
}




// parse file
LispList *LispParser::parse_file(const char *path){ return this->parse_file(std::string(path)); }
LispList *LispParser::parse_file(std::string path)
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
