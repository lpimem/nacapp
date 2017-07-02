#include <algorithm>
#include <string>

#include <ndn-cxx/util/time.hpp>

#include "thermometer.hpp"


namespace nacapp {

namespace thermometer {

Thermometer::Thermometer(string localPrefix,
                         string location,
                         string dataType,
                         shared_ptr<Face> f,
                         const string& producerDbPath,
                         shared_ptr<KeyChain> kc)
  : m_location(location)
  , m_dataType(dataType)
{
  Name prefix(localPrefix);
  prefix.append(NAME_COMPONENT_SAMPLE);
  prefix.append(m_dataType);
  m_node = make_unique<Node>(prefix, f, kc);

  Name managerPrefix(localPrefix);
  m_producer = make_shared<Producer>(managerPrefix, m_dataType, *f, producerDbPath);
}

void
Thermometer::run()
{
  registerPrefixes();
  m_node->serveForever();
}

// Input prefix should be in form :
//    /local-home/location/some/place
// Resulting prefix for this app will be:
//    /local-home/location/some/place/SAMPLE/temperature
void
Thermometer::setPrefix(Name prefix, Name location)
{
  prefix.append(location);
  prefix.append(m_dataType);
  m_node->setPrefix(prefix);
}

bool
Thermometer::onGetTemperature(const Interest& interest,
                              const Name& args,
                              shared_ptr<Data> data,
                              InterestShower show,
                              PutData put)
{
  std::string timeExpr = args.get(0).toUri();
  timeExpr.erase(std::remove(timeExpr.begin(), timeExpr.end(), '/'), timeExpr.end());
  time::system_clock::TimePoint timeslot = time::fromIsoString(timeExpr);
  time::system_clock::TimePoint now = time::system_clock::now();

  if (timeslot > now) {
    now = timeslot;
  }

  ndn::time::duration<double> sec = now - timeslot;
  if (sec.count() > QUERY_TIME_DURATION) {
    LOG(WARNING) << "Queried time is out-dated, will reply for current time as data content.";
  }

  int t = readTemp();
  auto onError = std::bind(&Thermometer::onNACProduceError, this, _1, _2);
  uint8_t content[] = {(uint8_t)t};
  m_producer->produce(*data, timeslot, content, sizeof(content), onError);
  return false;
}

// args: timeslot
// TODO: how to derive the correct ckey name from a data packet?
bool
Thermometer::onGetContentKey(const Interest& interest,
                             const Name& args,
                             shared_ptr<Data> data,
                             InterestShower show,
                             PutData put)
{
  std::string timeExpr = args.get(0).toUri();
  timeExpr.erase(std::remove(timeExpr.begin(), timeExpr.end(), '/'), timeExpr.end());
  time::system_clock::TimePoint timeslot = time::fromIsoString(timeExpr);
  Name keyName =
    m_producer->createContentKey(timeslot,
                                 std::bind(&Thermometer::onContentKeyEncrypted, this, put, interest, _1),
                                 std::bind(&Thermometer::onNACProduceError, this, _1, _2));
  LOG(INFO) << "creating content key: " << keyName;
  return true;
}

void
Thermometer::onContentKeyEncrypted(PutData put, const Interest& interest, const std::vector<Data>& d)
{
  LOG(INFO) << "content key retrieved: ";
  for (auto one : d) {
    LOG(INFO) << "    " << one.getName();
    if (one.getName() == interest.getName()) {
      put(make_shared<Data>(one));
      break;
    }
  }
  LOG(ERROR) << "no encrypted content key found for " << interest.getName();
}

void
Thermometer::onNACProduceError(const ndn::gep::ErrorCode& code, const std::string& msg)
{
  LOG(ERROR) << "Error producing data using NAC" << msg;
}

void
Thermometer::registerPrefixes()
{
  m_node->route(NAME_COMPONENT_C_KEY.toUri(),
                std::bind(&Thermometer::onGetContentKey, this, _1, _2, _3, _4, _5));

  m_node->route(NODE_DEFAULT_PATH,
                std::bind(&Thermometer::onGetTemperature, this, _1, _2, _3, _4, _5));
}

int
Thermometer::readTemp()
{
  // TODO: read from raspberry pi
  return 76;
}
} // thermometer
} // nacapp