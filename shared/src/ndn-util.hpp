#ifndef NDN_UTIL_HPP
#define NDN_UTIL_HPP

#include <string>

#include "common-cxx.hpp"
#include "common-ndn.hpp"

namespace nacapp {

extern void
expressInterest(shared_ptr<Face> f,
                Interest interest,
                const int retry,
                ndn::DataCallback onData,
                ndn::NackCallback onNack);

namespace data {

const std::string DEFAULT_TIME_FORMAT = "";

extern void
setStringContent(Data& d, const std::string& content);

extern string
getAsString(const Data& data);

extern time::system_clock::TimePoint
parseTimePoint(std::string expr);
} // namespace data
} // namespace nacapp
#endif /* NDN_UTIL_HPP */
