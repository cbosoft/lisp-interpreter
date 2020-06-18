#pragma once
#include <string>
#include <sstream>
#include <regex>

#include "../types.hpp"
#include "../formatter.hpp"
#include "../debug.hpp"
#include "../exception.hpp"
#include "../exception_check.hpp"
#include "../singletons.hpp"



class LispFunc_format : public virtual LispBuiltin {
  private:
    inline static const std::string name = "format";
    inline static const std::string doc =
      "(format format-string &rest objects)";

  public:
    LispFunc_format()
    {
    }

    const std::string repr() const
    {
      return "Func(" + this->name + ",builtin";
    }

    const std::string get_doc() const
    {
      return this->doc;
    }

    LispObject_ptr eval(LispList_ptr arg, LispEnvironment_ptr env) const
    {
      (void) env;
      narg_check_min(arg, 2, this->name, "format-string &rest objects");

      auto fmt_obj = arg->first();
      type_check_atom(fmt_obj, LISPATOM_STRING, this->name, "format-string");

      std::vector<std::string> objs_to_format;
      auto it = arg->begin();
      it++;
      objs_to_format.push_back((*it)->str());
      debug_message(Formatter() << "fmt_obj " << (*it)->str());
      it++;
      for (; it != arg->end(); ++it) {
        objs_to_format.push_back((*it)->str());
        debug_message(Formatter() << "fmt_obj " << (*it)->str());
      }

      std::string fmt_str = fmt_obj->get_value_atom()->get_value_string();
      std::regex re("\\{(\\d+)\\}");
      auto beg = std::sregex_iterator(fmt_str.begin(), fmt_str.end(), re);
      auto end = std::sregex_iterator();

      std::stringstream ss;
      auto cdata = fmt_str.data();
      int i = 0;
      for (auto it = beg; it != end; it++) {
        auto match = (*it);

        for (; i < match.position(); i++)
          ss << cdata[i];

        i += match.length();
        std::string obj_index_str = match[1];
        size_t obj_index = std::stoi(obj_index_str);
        
        if (obj_index >= objs_to_format.size())
          throw FormatError(Formatter() << "In format: format-object specifier out of range (" << obj_index << " > " << objs_to_format.size()-1 << ")");

        ss << objs_to_format[obj_index];
      }

      for (; i < (int)fmt_str.size(); i++)
        ss << cdata[i];

      std::string formatted = ss.str();
      return std::make_shared<LispObject>(formatted);
    }
};
