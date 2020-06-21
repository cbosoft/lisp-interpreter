#pragma once

#include <string>
#include <memory>

#include "../traceable/traceable.hpp"

class LispToken;
typedef std::shared_ptr<LispToken> LispToken_ptr;
class LispToken : public Traceable {
  private:
    std::string token;

  public:
    LispToken_ptr next;

    LispToken(std::string token);
    std::string get_token() {return this->token; }
    int compare(std::string s) { return this->token.compare(s); }
    void print();
};
