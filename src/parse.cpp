#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <ios>

#include "types.hpp"
#include "debug.hpp"
#include "tokenise.hpp"
#include "formatter.hpp"



LispParser::LispParser() {
  this->string_is_int_re = std::regex("^-?\\d+$");
  this->string_is_float_re = std::regex("^-?\\d+(\\.|e\\+|e-|e)\\d+$");
  this->string_is_string_re = std::regex("\".*\"", std::regex::extended);

  std::string home(getenv("HOME"));
  std::stringstream ss;

  ss << "./lib/";
  this->module_paths.push_back(ss.str());
  ss.str("");

  ss << home << "/.crisp/";
  this->module_paths.push_back(ss.str());
  ss.str("");

  ss << "/lib/crisp/";
  this->module_paths.push_back(ss.str());
  ss.str("");

}


int LispParser::string_is_int(std::string s) 
{ 
  return std::regex_match(s, this->string_is_int_re); 
}

int LispParser::string_is_float(std::string s) 
{ 
  return std::regex_match(s, this->string_is_float_re); 
}

int LispParser::string_is_string(std::string s) 
{ 
  return std::regex_match(s, this->string_is_string_re); 
}






// Create new object, guessing type
LispObject_ptr LispParser::new_object_guess_type(LispToken_ptr t) {
  std::string s = t->get_token();

  debug_message(Formatter() << "GUESSING " << s);
  LispObject_ptr rv;
  
  if (s.compare("(") == 0) {
    debug_message(Formatter() << "GUESSING "<< s << "is LIST");
    LispList_ptr newlist = std::make_shared<LispList>();
    rv = std::make_shared<LispObject>(newlist);
  }
  else if (this->string_is_int(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is INT");
    LispAtom_ptr newatom = std::make_shared<LispAtom>(std::stol(s));
    rv = std::make_shared<LispObject>(newatom);
  }
  else if (this->string_is_float(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is FLOAT");
    LispAtom_ptr newatom = std::make_shared<LispAtom>(std::stof(s));
    rv = std::make_shared<LispObject>(newatom);
  }
  else if (this->string_is_string(s)) {
    debug_message(Formatter() << "GUESSING " << s << " is STRING");
    std::string body = s.substr(1, s.length()-2);
    LispAtom_ptr newatom = std::make_shared<LispAtom>(body);
    rv = std::make_shared<LispObject>(newatom);
  }
  else {
    debug_message(Formatter() << "GUESSING " << s << " is SYMBOL");
    LispSymbol_ptr newsymbol = std::make_shared<LispSymbol>(s);
    rv = std::make_shared<LispObject>(newsymbol);
  }

  rv->inherits_from(t);
  return rv;
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
      LispList_ptr newlist = std::make_shared<LispList>();
      new_obj = std::make_shared<LispObject>(newlist);
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
LispList_ptr LispParser::parse_string(char *char_arr, TraceSource proto_source){ return this->parse_string(std::string(char_arr), proto_source); }
LispList_ptr LispParser::parse_string(std::string s, TraceSource proto_source)
{

  LispToken_ptr tokens = tokenise(s, proto_source);

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
  if (ifs.fail()) {
    throw IOError(Formatter() << "Could not open file \"" << path << "\" (" << errno << ") " << strerror(errno));
  }

  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string file_contents = buffer.str();
  auto it = file_contents.begin();

  if (!file_contents.compare(0, 2, "#!")) {
    for (;it != file_contents.end() && (*it) != '\n'; ++it);
    file_contents.erase(file_contents.begin(), it);
  }

  TraceSource proto_source = {.type=TRACESOURCE_FILE, .path_or_commands=path, .row=0, .column=0, .token_length=0};
  return this->parse_string(file_contents, proto_source);
}




// parse module
LispList_ptr LispParser::parse_module(const char *name){ return this->parse_module(std::string(name)); }
LispList_ptr LispParser::parse_module(std::string name)
{
  debug_message("in module parse");
  std::string path = this->search_module(name);
  debug_message(Formatter() << "found path " << path);
  return this->parse_file(path);
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
