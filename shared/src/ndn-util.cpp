#include <chrono>
#include <iomanip>
#include <thread>

#include <ndn-cxx/lp/nack.hpp>

#include "ndn-util.hpp"

namespace nacapp {

void
onTimeout(shared_ptr<Face> f,
          std::shared_ptr<int> retryCount,
          ndn::DataCallback onData,
          ndn::NackCallback onNack,
          const Interest& interest)
{
  if ((*retryCount)-- > 0) {
    const int delay = 500;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    ndn::TimeoutCallback retry = std::bind(&onTimeout, f, retryCount, onData, onNack, _1);
    f->expressInterest(interest, onData, onNack, retry);
  }
  else {
    ndn::lp::Nack n;
    onNack(interest, n);
  }
}

void
expressInterest(shared_ptr<Face> f,
                Interest interest,
                const int retry,
                ndn::DataCallback onData,
                ndn::NackCallback onNack)
{
  shared_ptr<int> retryCount = make_shared<int>(retry);
  ndn::TimeoutCallback onTimeoutRetry = std::bind(&onTimeout, f, retryCount, onData, onNack, _1);
  LOG(INFO) << "Express: " << interest.toUri();
  f->expressInterest(interest, onData, onNack, onTimeoutRetry);
}

namespace data {

void
setStringContent(Data& d, const std::string& content)
{
  d.setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.length());
}

string
getAsString(const Data& data)
{
  const uint8_t* bytes = data.getContent().value();
  const int len = data.getContent().value_size();
  std::string s(reinterpret_cast<char const*>(bytes), len);
  return s;
}

time::system_clock::TimePoint
parseTimePoint(const std::string& expr, const std::string& format)
{
  std::tm tm = {};
  std::stringstream ss(expr);
  ss >> std::get_time(&tm, format.c_str());
  return time::system_clock::from_time_t(std::mktime(&tm));
}
} // namespace data
} // namespace nacapp