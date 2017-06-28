#include <iomanip>

#include "ndn-util.hpp"

namespace nacapp {
namespace data {

void
setStringContent(Data& d, const std::string& content)
{
  d.setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.length());
}

time::system_clock::TimePoint
parseTimePoint(const std::string& expr, const std::string& format)
{
  std::tm tm = {};
  std::stringstream ss(expr);
  ss >> std::get_time(&tm, format.c_str());
  return time::system_clock::from_time_t(std::mktime(&tm));
}
}
}