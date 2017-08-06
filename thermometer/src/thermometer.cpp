#include <algorithm>
#include <string>

#include <ndn-cxx/util/string-helper.hpp>
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

time::system_clock::TimePoint
parseTimeSlot(string timeExpr, time::system_clock::TimePoint now)
{
  time::system_clock::TimePoint timeslot;
  timeExpr.erase(std::remove(timeExpr.begin(), timeExpr.end(), '/'), timeExpr.end());
  timeslot = time::fromIsoString(timeExpr);
  if (timeslot > now) {
    timeslot = now;
  }
  return timeslot;
}

bool
Thermometer::onGetTemperature(const Interest& interest,
                              const Name& args,
                              shared_ptr<Data> data,
                              InterestShower show,
                              PutData put)
{
  time::system_clock::TimePoint timeslot;
  const time::system_clock::TimePoint now = time::system_clock::now();
  if (args.size() <= 0) {
    LOG(WARNING) << "query missing argument";
    timeslot = time::system_clock::now();
  }
  else {
    timeslot = parseTimeSlot(args.get(0).toUri(), now);
  }

  auto onError = std::bind(&Thermometer::onNACProduceError, this, _1, _2);
  auto doProduce = [=]() {
    int t = readTemp();
    string tStr = ndn::to_string(t);
    std::vector<uint8_t> bytes(tStr.begin(), tStr.end());
    bytes.push_back('\0');
    uint8_t* content = &bytes[0];
    this->m_producer->produce(*data, timeslot, content, bytes.size(), onError);
    data->setName(interest.getName());
    put(data);
  };
  auto onKeyEncrypted = [=](const std::vector<Data>& d) {
    for (auto oneKey : d) {
      Name keyName{oneKey.getName()};
      this->m_ckeys.insert(std::pair<Name, shared_ptr<Data>>{keyName, make_shared<Data>(oneKey)});
    }
    doProduce();
  };

  LOG(INFO) << "Thermometer::onGetTemperature";
  Name kname = m_producer->createContentKey(timeslot, onKeyEncrypted, onError);
  if (nullptr != searchCKey(kname)) {
    LOG(INFO) << "Using existing key";
    doProduce();
  }
  return true;
}

// args: timeslot
bool
Thermometer::onGetContentKey(const Interest& interest,
                             const Name& args,
                             shared_ptr<Data> data,
                             InterestShower show,
                             PutData put)
{
  if (args.size() < 1) {
    data->setContentType(ndn::tlv::ContentType_Nack);
    return false;
  }

  LOG(INFO) << "Thermometer::onGetContentKey";
  std::string timeExpr = args.get(0).toUri();
  timeExpr.erase(std::remove(timeExpr.begin(), timeExpr.end(), '/'), timeExpr.end());
  time::system_clock::TimePoint timeslot = time::fromIsoString(timeExpr);

  auto onError = std::bind(&Thermometer::onNACProduceError, this, _1, _2);
  Name keyName = m_producer->createContentKey(timeslot, nullptr, onError);
  shared_ptr<Data> key = searchCKey(keyName);
  if (nullptr == key) {
    LOG(ERROR) << "cannot find ckey " << keyName.toUri();
  }
  else {
    put(key);
  }
  return false;
}

void
Thermometer::onContentKeyEncrypted(PutData put, const Interest& interest, const std::vector<Data>& d)
{
  LOG(INFO) << "content key retrieved: [" << d.size() << "]";

  const Name interestName = interest.getName();
  shared_ptr<Data> contentKey = nullptr;
  string hex;
  for (auto one : d) {
    LOG(INFO) << "    " << one.getName();
    if (interestName.isPrefixOf(one.getName())) {
      contentKey = make_shared<Data>(one);
      LOG(INFO) << "    "
                << "    "
                << "MATCH";
      string keyHex = toHex(*one.getContent().getBuffer(), true);
      std::cout << keyHex << std::endl
                << "EQUALS prev? " << (keyHex == hex) << std::endl
                << "- - - - - - - -" << std::endl;
      hex = keyHex;
    }
  }
  if (contentKey != nullptr) {
    put(contentKey);
    return;
  }
  LOG(ERROR) << "no encrypted content key found for " << interestName;
  auto nack = make_shared<Data>();
  nack->setName(interestName);
  nack->setContentType(ndn::tlv::ContentType_Nack);
  put(nack);
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

shared_ptr<Data>
Thermometer::searchCKey(const Name& ckey)
{
  LOG(INFO) << "searchCKey ... ";
  auto found = m_ckeys.find(ckey);
  if (found != m_ckeys.end()) {
    return found->second;
  }
  for (auto pair : m_ckeys) {
    Name keyName = pair.first;
    if (ckey.isPrefixOf(keyName)) {
      return pair.second;
    }
  }
  return nullptr;
}

} // namespace thermometer
} // namespace nacapp