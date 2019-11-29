#include <string>
#include <iostream>
#include <regex>
#include <vector>

#include "types.hpp"
#include "debug.hpp"






// Create new object, guessing type
LispObject *LispParser::new_object_guess_type(LispToken *t) {
  std::string s = t->get_token();

  debug_message("GUESSING %s\n", s.c_str());
  
  if (s.compare("(") == 0) {
    debug_message("GUESSING %s is LIST\n", s.c_str());
    LispListElement *newlist = new LispListElement();
    return new LispObject(newlist);
  }

  if (this->string_is_int(s)) {
    debug_message("GUESSING %s is INT\n", s.c_str());
    LispAtom *newatom = new LispAtom( atol(s.c_str()) );
    return new LispObject(newatom);
  }

  if (this->string_is_float(s)) {
    debug_message("GUESSING %s is FLOAT\n", s.c_str());
    LispAtom *newatom = new LispAtom( atof(s.c_str()) );
    return new LispObject(newatom);
  }


  if (this->string_is_string(s)) {
    debug_message("GUESSING %s is STRING\n", s.c_str());
    std::string body = s.substr(1, s.length()-1);
    LispAtom *newatom = new LispAtom(body);
    return new LispObject(newatom);
  }

  debug_message("GUESSING %s is SYMBOL\n", s.c_str());
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
    debug_message("TOKEN: %s\n", token->get_token().c_str() );

    if (token->compare("(") == 0) {
      debug_message("OPENING NEW LIST\n");
      LispListElement *newlist = new LispListElement();
      new_obj = new LispObject(newlist);
      debug_message("after OPENING NEW LIST\n");
      open_lists.top()->append(new_obj);
      open_lists.push(newlist);
    }
    else if (token->compare(")") == 0) {
      debug_message("CLOSING LIST\n");
      open_lists.pop();
    }
    else {
      debug_message("GUESS\n");
      new_obj = this->new_object_guess_type(token);

      //debug_message("NEW OBJECT %s (%s)\n", LispObject_repr(new), LispObject_type(new));
      open_lists.top()->append(new_obj);
    }

  }

  return root;
}
