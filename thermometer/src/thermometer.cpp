#include "thermometer.hpp"

namespace nacapp {

namespace thermometer {

Thermometer::Thermometer(string localPrefix,
                         string location,
                         shared_ptr<Face> f,
                         const string& producerDbPath)
  : m_location(location)
  , m_dataType("temperature")
{
  Name prefix(localPrefix);
  prefix.append(location);
  m_node = make_unique<Node>(prefix, f);
  m_producer = make_unique<Producer>(prefix, m_dataType, *f, producerDbPath);
}

void
Thermometer::run()
{
  registerPrefixes();
  m_node->serveForever();
}

void
Thermometer::setPrefix(Name prefix)
{
  m_node->setPrefix(prefix);
}

void
Thermometer::onGetTemperature(const Interest& interest,
                              const Name& args,
                              shared_ptr<Data> data,
                              InterestShower show,
                              PutData put)
{
  const int t = readTemp();
  auto now = time::system_clock::now();
  m_producer->produce(*data,
                      now,
                      (uint8_t*)t,
                      1,
                      std::bind(&Thermometer::onNACProduceError, this, _1, _2));
}

// args: timeslot
// TODO: how to derive the correct ckey name from a data packet?
void
Thermometer::onGetContentKey(const Interest& interest,
                             const Name& args,
                             shared_ptr<Data> data,
                             InterestShower show,
                             PutData put)
{
  Name timeslotArg{args.at(0)};
  std::string timeExpr = timeslotArg.toUri();
  time::system_clock::TimePoint timeslot = time::fromIsoString(timeExpr);
  m_producer->createContentKey(timeslot,
                               std::bind(&Thermometer::onContentKeyEncrypted, this, put, interest, _1),
                               std::bind(&Thermometer::onNACProduceError, this, _1, _2));
}

void
Thermometer::onContentKeyEncrypted(PutData put, const Interest& interest, const std::vector<Data>& d)
{
  LOG(DEBUG) << "content key retrieved: ";
  for (auto one : d) {
    LOG(DEBUG) << "    " << one.getName();
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
  LOG(ERROR) << "Error producing data using NAC" << code << ": " << msg;
}

void
Thermometer::registerPrefixes()
{
  m_node->route(m_dataType, std::bind(&Thermometer::onGetTemperature, this, _1, _2, _3, _4, _5));

  const std::string ckeyPath = m_dataType + "/C-KEY";
  m_node->route(ckeyPath, std::bind(&Thermometer::onGetContentKey, this, _1, _2, _3, _4, _5));
}

int
Thermometer::readTemp()
{
  // TODO: read from raspberry pi
  return 76;
}
} // thermometer
} // nacapp