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
    LispListElement *newlist = new LispListElement();
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
LispListElement *LispParser::parse(LispToken *tokens)
{
  LispListElement *root = new LispListElement();
  LispObject *new_obj = NULL;
  std::stack<LispListElement *> open_lists;
  open_lists.push(root);

  debug_message("IN PARSE\n");

  for (LispToken *token = tokens; token != NULL; token = token->next) {
    debug_message(Formatter() << "TOKEN: " << token->get_token());

    if (token->compare("(") == 0) {

      debug_message("OPENING NEW LIST\n");
      LispListElement *newlist = new LispListElement();
      new_obj = new LispObject(newlist);
      open_lists.top()->append(new_obj);
      open_lists.push(newlist);
      debug_message("after OPENING NEW LIST\n");

    }
    else if (token->compare(")") == 0) {

      debug_message("CLOSING LIST\n");
      open_lists.pop();

    }
    else {

      debug_message("GUESS\n");
      new_obj = this->new_object_guess_type(token);

      debug_message(Formatter() << "NEW OBJECT " << new_obj->repr());
      open_lists.top()->append(new_obj);

    }

  }

  return root;
}




// parse
LispListElement *LispParser::parse_string(std::string s)
{

  LispToken *tokens = tokenise(s);

  debug_message("TOKENS:");
  for (LispToken *i = tokens; i != NULL; i = i->next) {
    debug_message(Formatter() << "| " << i->get_token());
  }

  return this->parse(tokens);
}




// parse file
LispListElement *LispParser::parse_file(std::string path)
{
  std::ifstream ifs(path);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  return this->parse_string(buffer.str());
}
