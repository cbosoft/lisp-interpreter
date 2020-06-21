#pragma once

#include <regex>
#include <list>
#include <string>

#include "../object/object.hpp"
#include "../token/token.hpp"

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
