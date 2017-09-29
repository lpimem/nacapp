//
// Created by Lei Pi on 2/26/17.
//

#include <stdlib.h>
#include <time.h>

#include "string_helpers.hpp"

namespace nacapp {

template<typename Out>
void
split(const std::string& s, char delim, Out result)
{
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string>
split(const std::string& s, char delim)
{
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

std::string
randomHex()
{
  srand(time(NULL));
  int random = rand();
  std::stringstream stream;
  stream << std::hex << random;
  return stream.str();
}

} // namespace nacapp