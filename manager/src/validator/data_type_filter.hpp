#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <set>
#include <string>

namespace nacapp {

class DataTypeFilter {

public:
  DataTypeFilter() {}
  DataTypeFilter(std::initializer_list<std::string> list) : m_allowed(list) {}

public:
  void allow(const std::string &dataType) { m_allowed.insert(dataType); }

  bool isAllowed(const std::string &dataType) {
    return m_allowed.find(dataType) != m_allowed.end();
  }

private:
  std::set<std::string> m_allowed;
};
} // nacapp

#endif /* DATA_TYPES_HPP */
