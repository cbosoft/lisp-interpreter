#pragma once

#include "../printable/printable.hpp"
#include "../executable/executable.hpp"
#include "../documented/documented.hpp"
#include "../list/list.hpp"
#include "../symbol/symbol.hpp"
#include "../object/object.hpp"

class LispFunction :
  virtual public Printable,
  virtual public Executable,
  virtual public Documented
{

  private:
    std::vector<std::string> arg_names;
    std::vector<LispObject_ptr> arg_default_values;
    LispList_ptr body;
    std::string name;
    std::string doc;
    void add_arg(std::string arg){ this->arg_names.push_back(arg); }

    // TODO: remove this heinous crime
    mutable LispObject_ptr result;

  public:
    LispFunction() : Executable() {};
    LispFunction(LispList_ptr arglist, LispList_ptr defvalue_list, LispList_ptr body, std::string name, std::string doc, bool is_macro=false)
      : Executable(), body(body), name(name), doc(doc)
    {
      this->body = body;
      this->name = name;
      this->doc = doc;

      for (auto iter = arglist->begin(); iter != arglist->end(); ++iter) {
        this->add_arg( (*iter)->get_value_symbol()->get_name() );
      }

      for (auto iter = defvalue_list->begin(); iter != defvalue_list->end(); ++iter) {
        this->arg_default_values.push_back(*iter);
      }

      if (is_macro)
        this->set_macro();
    };
    LispFunction(LispList_ptr arglist, LispList_ptr body, std::string name, std::string doc, bool is_macro=false)
      : LispFunction(arglist, std::make_shared<LispList>(), body, name, doc, is_macro)
    {
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const;
    const std::string repr() const { return this->body->repr(); }
    const std::string str() const {return this->body->str(); }
    const std::string get_doc() const override { return this->doc; }

    LispObject_ptr get_result() { return this->result ? this->result : std::make_shared<LispObject>(std::make_shared<LispList>()); }
};
