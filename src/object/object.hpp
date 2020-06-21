#pragma once

#include "../types.hpp"
#include "../printable/printable.hpp"
#include "../traceable/traceable.hpp"
#include "../documented/documented.hpp"
#include "object_types.hpp"

class LispObject :
  virtual public Printable,
  virtual public Traceable,
  virtual public Documented
{

  private:
    LispAtom_ptr value_atom;
    LispList_ptr value_list;
    LispSymbol_ptr value_symbol;
    LispObject_Type type;
    std::string doc;

  public:
    LispObject();

    const std::string str() const override;
    const std::string repr() const override;
    std::string repr_type() const;
    std::string static repr_type(LispObject_Type type);

    LispObject_Type get_type() const { return this->type; }
    LispObject_ptr eval(LispEnvironment_ptr env);

    LispObject(int atomval);
    LispObject(bool atomval);
    LispObject(long atomval);
    LispObject(double atomval);
    LispObject(std::string atomval);
    LispObject(LispAtom_ptr atom);
    LispObject(LispList_ptr list);
    LispObject(LispSymbol_ptr symbol);

    void set_value(LispAtom_ptr atom) {this->value_atom = atom;}
    void set_value(LispList_ptr list) {this->value_list = list; }
    void set_value(LispSymbol_ptr symbol) {this->value_symbol = symbol; }

    LispAtom_ptr get_value_atom() { return this->value_atom; }
    const LispAtom_ptr get_value_atom() const { return this->value_atom; }
    LispList_ptr get_value_list() { return this->value_list; }
    const LispList_ptr get_value_list() const { return this->value_list; }
    LispSymbol_ptr get_value_symbol() { return this->value_symbol; }
    const LispSymbol_ptr get_value_symbol() const { return this->value_symbol; }

    bool is_truthy(LispEnvironment_ptr env=nullptr);
    bool eq(const LispObject_ptr &other) const;

    const std::string get_doc() const override { return (this->doc.size() ? this->doc : this->repr()); }
    void set_doc(std::string s) { this->doc = s; }

};
