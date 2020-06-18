#include <string>
#include <iostream>
#include <sstream>
#include <memory>


#include "tokenise.hpp"
#include "exception.hpp"
#include "formatter.hpp"

#define IS_WHITESPACE(C) ((C == '\n') || (C == ' ') || (C == '\t'))

#define ADD_TO_TOKENS(KW) \
  new_token = std::make_shared<LispToken>(LispToken(KW));\
  new_source = proto_source;\
  new_source.row = row;\
  new_source.column = col;\
  new_source.token_length = string_length(KW);\
  new_token->set_source(make_ptr(new_source));\
  if (rv == NULL) {\
    rv = new_token;\
  }\
  else {\
    current_token->next = new_token;\
  }\
  current_token = new_token;


int string_length(std::string s)
{
  return s.size();
}

int string_length(const char *s)
{
  return strlen(s);
}


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
LispToken_ptr tokenise(std::string input, TraceSource proto_source)
{

  char ch, nch;
  unsigned long i = 0;

  std::stringstream kw_or_name_builder;
  bool in_quote=false, in_str=false, add_close_parens_on_break = false;
  int parens_level = 0, add_close_parens_on_parens = -1;
  int row = 0, col = 0;
  bool leading_space = true;
  TraceSource new_source;

  LispToken_ptr rv, current_token, new_token;

  for (i = 0, ch = input[0], nch=input[1]; i < input.length(); ch = input[++i], nch=input[i+1]) {
    col++;

    if ((input[i] == ';') and (not in_quote)) {
      for (;input[i] != '\n' && i < input.length(); i++);
      col = 0;
      row ++;
      leading_space = true;
      continue;
    }
    
    // if breaking char: space, newline, or parens
    if (( IS_WHITESPACE(ch) || (ch == ')') || (ch == '(') || (ch == '\'')) and (not in_quote) and (not in_str)) {

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
      switch (ch) {

        case '(':
          ADD_TO_TOKENS("(");
          parens_level++;
          break;

        case  ')':
          ADD_TO_TOKENS(")");
          parens_level--;

          if (add_close_parens_on_parens >= 0 && (parens_level == add_close_parens_on_parens)) {
            add_close_parens_on_parens = -1;
            ADD_TO_TOKENS(")");
          }
          break;

        case '\'':

          ADD_TO_TOKENS("(");
          ADD_TO_TOKENS("quote");

          if (nch == '('){
            //debug_message("NEXT CHAR IS '('; quote list\n");
            add_close_parens_on_parens = parens_level;
            debug_message(Formatter() << parens_level << "\n");
          }
          else if (IS_WHITESPACE(nch)) {
            //error
            //debug_message("NEXT CHAR IS WHITE SPACE! ERROR");
            //Exception_raise("SyntaxError", "tokenise", NULL, "single quote should be before a list or other object.");
            throw SyntaxError(Formatter() << "Single quote ' should be before an object: 'object = (quote object)");
          }
          else {
            add_close_parens_on_break = true;
            //debug_message("NEXT CHAR IS '('; quote kw\n");
          }
          break;

        case '\n':
          row ++;
          col = 0;
          break;

      }

    }
    else {

      switch (ch) {

        case '"':
          in_quote = !in_quote;
          in_str = !in_str;
          break;

        case '\n':
          leading_space = true;
          kw_or_name_builder << ch;
          break;
      }
      

      // TODO this is a bit messy; could definitely be tidied up
      if (in_str) {

        if (not (IS_WHITESPACE(ch) and leading_space)) {
          leading_space = false;
          kw_or_name_builder << ch;
        }

      }
      else {
        kw_or_name_builder << ch;
      }

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
