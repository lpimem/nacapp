#ifndef NDN_UTIL_HPP
#define NDN_UTIL_HPP

#include <string>

#include "common-ndn.hpp"

namespace nacapp {
namespace data {

const std::string DEFAULT_TIME_FORMAT = "";

extern void
setStringContent(Data& d, const std::string& content);

extern time::system_clock::TimePoint
parseTimePoint(std::string expr);
}
}
#endif /* NDN_UTIL_HPP */
