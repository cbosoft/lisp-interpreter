#pragma once
#include <string>


class LispToken {
  private:
    std::string token;

  public:
    LispToken *next;

    LispToken(std::string token);
    void print();
};


LispToken *tokenise(std::string input);
