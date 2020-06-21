#pragma once

#include "../util/formatter.hpp"
#include "../util/debug.hpp"
#include "traceable_types.hpp"


class Traceable {

  private:
    TraceSource_ptr source;
    const std::string get_file_trace() const;

  public:
    Traceable()
    {
      // do nothing
    }

    void set_source(const TraceSource_ptr& source)
    {
      this->source = source;
    }

    TraceSource_ptr get_source()
    {
      return this->source;
    }

    const TraceSource_ptr& get_source() const
    {
      return this->source;
    }

    const std::string repr_source();

    template<typename T>
    void inherits_from(std::shared_ptr<T> obj) {
      this->source = obj->get_source();
      debug_message(Formatter() << "inherited: " << this->repr_source());
    }

    //void inherits_from(std::shared_ptr<LispObject> obj);
};
