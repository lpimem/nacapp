#ifndef NAMED_SCHEDULE_HPP
#define NAMED_SCHEDULE_HPP

#include "common.hpp"

namespace nacapp {

class NamedInterval {

public:
  NamedInterval(Name name, RepetitiveInterval interval)
      : m_name(name), m_interval(interval) {}

  const Name &getName() const { return m_name; }

  const RepetitiveInterval &getInterval() const { return m_interval; }

private:
  Name m_name;
  RepetitiveInterval m_interval;
};

} // nacapp
#endif /* NAMED_SCHEDULE_HPP */
