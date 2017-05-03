#ifndef UTIL_HPP
#define UTIL_HPP

#include "common.hpp"

namespace nacapp {

namespace data {

string getAsString(const Data &data) {
  const uint8_t *bytes = data.getContent().value();
  const int len = data.getContent().value_size();
  std::string s(reinterpret_cast<char const *>(bytes), len);
  return s;
}

PublicKey getAsPublicKey(const Data &data) {
  PublicKey key(data.getContent().value(), data.getContent().value_size());
  return key;
}

} // data

} // nacapp
#endif /* UTIL_HPP */
