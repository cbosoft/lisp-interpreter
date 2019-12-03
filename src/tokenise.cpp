#include <string>
#include <iostream>
#include <sstream>
#include <memory>


#include "tokenise.hpp"

#define IS_WHITESPACE(C) ((C == '\n') || (C == ' ') || (C == '\t'))

#define ADD_TO_TOKENS(KW) \
  new_token = std::make_shared<LispToken>(LispToken(KW));\
  if (rv == NULL) {\
    rv = new_token;\
  }\
  else {\
    current_token->next = new_token;\
  }\
  current_token = new_token;




// LispToken constructor
LispToken::LispToken(std::string token)
{
  this->token = token;
  this->next = NULL;
}




// LispToken print method
void LispToken::print()
{
  std::cout << this->token << " ";
  if (this->next != NULL)
    this->next->print();
  else
    std::cout << std::endl;
}



// Convert input into list of tokens
LispToken_ptr tokenise(std::string input)
{

  char ch, nch;
  unsigned long i = 0;

  std::stringstream kw_or_name_builder;
  bool in_quote = 0, add_close_parens_on_break = 0, add_close_parens_on_parens = 0;
  int parens_level = 0;

  LispToken_ptr rv, current_token, new_token;

  for (i = 0, ch = input[0], nch=input[1]; i < input.length(); ch = input[++i], nch=input[i+1]) {

    if (input[i] == ';') {
      for (;input[i] != '\n' && i < input.length(); i++);
      continue;
    }
    
    // if breaking char: space, newline, or parens
    if (( IS_WHITESPACE(ch) || (ch == ')') || (ch == '(') || (ch == '\'')) && !in_quote) {

      // finish reading keyword or name
      if (kw_or_name_builder.str().length()) {
        ADD_TO_TOKENS(kw_or_name_builder.str());
        kw_or_name_builder.str("");

        if (add_close_parens_on_break) {
          add_close_parens_on_break = 0;
          ADD_TO_TOKENS(")");
        }
      }

      // TODO switch-case
      // action needed on breaking char?
      if (ch == '(') {
        ADD_TO_TOKENS("(");
        parens_level++;
      }
      if (ch == ')') {
        ADD_TO_TOKENS(")");
        parens_level--;

        if (add_close_parens_on_parens) {
          add_close_parens_on_parens = 0;
          ADD_TO_TOKENS(")");
        }
      }
      else if (ch == '\'') {

        ADD_TO_TOKENS("(");
        ADD_TO_TOKENS("quote");

        if (nch == '('){
          //debug_message("NEXT CHAR IS '('; quote list\n");
          add_close_parens_on_parens = 1;
        }
        else if (IS_WHITESPACE(nch)) {
          //error
          //debug_message("NEXT CHAR IS WHITE SPACE! ERROR");
          //Exception_raise("SyntaxError", "tokenise", NULL, "single quote should be before a list or other object.");
        }
        else {
          add_close_parens_on_break = 1;
          //debug_message("NEXT CHAR IS '('; quote kw\n");
        }
      }

    }
    else {

      if (ch == '"')
        in_quote = !in_quote;

      kw_or_name_builder << ch;

    }

  }

  if (kw_or_name_builder.str().length()) {
    ADD_TO_TOKENS(kw_or_name_builder.str());
    kw_or_name_builder.str();

    if (add_close_parens_on_break) {
      add_close_parens_on_break = 0;
      ADD_TO_TOKENS(")");
    }
  }

  return rv;
}
