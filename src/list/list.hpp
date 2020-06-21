#pragma once

#include <list>

#include "../printable/printable.hpp"
#include "../types.hpp"

class LispList : virtual public Printable {
  private:
    std::list<LispObject_ptr> obj_list;
    std::list<LispObject_ptr>::const_iterator it;

  public:
    LispList(){};

    LispList(std::list<LispObject_ptr>::const_iterator from, std::list<LispObject_ptr>::const_iterator to){ 
      this->obj_list = std::list<LispObject_ptr>(from, to);
    }

    std::list<LispObject_ptr>::const_iterator end() const { return this->obj_list.end(); }
    std::list<LispObject_ptr>::const_iterator begin() const { return this->obj_list.begin(); }
    LispObject_ptr first() { return this->obj_list.front(); }
    LispObject_ptr next(bool restart = false) {
      if (restart) {
        this->it = this->begin();
      }
      else {
        this->it++;
      }

      return (*this->it);
    }

    LispList_ptr rest(int n = 1) { 
      auto it = this->begin();
      for (int i = 0; i < n; i++, ++it);

      if (it == this->end())
        return std::make_shared<LispList>();

      return std::make_shared<LispList>(it, this->end()); 
    }

    int count() const { return this->obj_list.size(); }
    void append(LispObject_ptr next_value) { this->obj_list.push_back(next_value); }
    std::string repr();
    std::string str();
    bool eq(const LispList_ptr &other) const;

    LispObject_ptr eval_each(LispEnvironment_ptr env) const;
};
