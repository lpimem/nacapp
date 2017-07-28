#include <ndn-cxx/encoding/tlv.hpp>

#include "service.hpp"
#include "util.hpp"

namespace nacapp {

namespace manager {


std::vector<Name>
parseReadArgs(const Name& args)
{
  size_t pos = 0;
  for (; pos < args.size(); pos++) {
    ndn::name::Component c = args.get(pos);
    if (c == NAME_COMPONENT_E_KEY || c == NAME_COMPONENT_D_KEY) {
      break;
    }
  }
  Name location = args.getSubName(0, pos);
  Name keyType = args.getSubName(pos, 1);
  Name keyArgs = args.getSubName(pos + 1);
  return std::vector<Name>{location, keyType, keyArgs};
}
}

bool
Service::onRead(const Interest& interest,
                const Name& args,
                shared_ptr<Data> data,
                InterestShower show,
                PutData put)
{
  std::vector<Name> parts = manager::parseReadArgs(args);
  const Name location = parts[0];
  const Name keyType = parts[1];
  const Name keyArgs = parts[2];
  if (keyType.toUri() == "/E-KEY")
    return onGetEKey(interest, location, keyArgs, data, show, put);
  else if (keyType.toUri() == "/D-KEY")
    return onGetDKey(interest, location, keyArgs, data, show, put);
  else {
    string msg{"invalid pattern for path /READ"};
    LOG(ERROR) << msg;
    data->setContentType(ndn::tlv::ContentType_Nack);
    data->setContent(reinterpret_cast<const uint8_t*>(msg.c_str()), msg.length());
    return false;
  }
}

bool
Service::onGetEKey(const Interest& interest,
                   const Name& datatype,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower want,
                   PutData put)
{
  if (datatype.size() < 1) {
    throw "onGetEKey requires a valid datatype, got : " + datatype.toUri();
  }
  ndn::Exclude exclude = interest.getExclude();
  const string timeExpr = exclude.begin()->from.toUri();
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timeExpr);
  shared_ptr<Data> ekey = m_manager->getEKey(datatype, timeslot);
  if (ekey == nullptr) {
    data->setContentType(ndn::tlv::ContentType_Nack);
    return false;
  }
  // auto period = time::milliseconds(1000 * 60 * 24);
  // TODO: Set
  auto debugPeriod = time::milliseconds(1000);
  nacapp::data::setFreshnessPeriod(*ekey, debugPeriod);
  LOG(INFO) << "[DEBUG] E-Key Freshness Period: " << ekey->getFreshnessPeriod();
  put(ekey);
  return false;
}

bool
Service::onGetDKey(const Interest& interest,
                   const Name& datatype,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower want,
                   PutData put)
{
  if (args.size() < 4) {
    LOG(ERROR) << "Get D-KEY: Missing arguments";
    data->setContentType(ndn::tlv::ContentType_Nack);
    return false;
  }
  LOG(INFO) << "[DEBUG] D-KEY Args : " << args.toUri();
  Name entity = args.getSubName(3);
  LOG(INFO) << "[DEBUG] D-KEY Entity : " << entity.toUri();
  const string timeExpr = args.get(0).toUri();
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timeExpr);
  shared_ptr<Data> dkey = m_manager->getDKey(datatype, entity, timeslot);
  if (nullptr == dkey) {
    data->setContentType(ndn::tlv::ContentType_Nack);
    return false;
  }
  dkey->setFreshnessPeriod(time::milliseconds(1000));
  put(dkey);
  LOG(INFO) << "DEBUG: onGetDKey: 4 Key published.";
  return false;
}

bool
Service::onGetIdentityKey(const Interest& interest,
                          const Name& args,
                          shared_ptr<Data> data,
                          InterestShower want,
                          PutData put)
{
  Name entity{args};
  shared_ptr<Certificate> cert = m_manager->getIdentity(entity);
  if (nullptr == cert) {
    data->setContentType(ndn::tlv::ContentType_Nack);
  }
  else {
    Certificate copy{*cert};
    LOG(INFO) << "Certificate name changed " << std::endl
              << "From: " << cert->getName().toUri() << std::endl
              << "To: " << interest.getName() << std::endl
              << "KeyName: " << cert->getKeyName() << std::endl;
    copy.setName(interest.getName());
    put(make_shared<Certificate>(copy));
  }
  return false;
}

bool
Service::onAddIdentity(const Interest& interest,
                       const Name& args,
                       shared_ptr<Data> data,
                       InterestShower want,
                       PutData put)
{
  Name entity{args};
  authenticateManagementInterest(interest, entity); //todo
  want(entity, std::bind(&Service::onAddIdentityKey, this, interest, data, put, _1));
  return true;
}

void
Service::onAddIdentityKey(const Interest& interest,
                          shared_ptr<Data> data,
                          PutData put,
                          const Data& keyData)
{
  // TODO: should let manager sign the key
  // Buffer pubkey = parseIdentityPubKey(keyData);
  // Certificate cert = signPubkey(keyData.getName(), pubkey);
  // ----
  Certificate cert(keyData);
  data->setName(interest.getName());
  nacapp::data::setStringContent(data, cert.getName().toUri());
  LOG(INFO) << "[DEBUG] onAddIdentityKey: new cert: " << cert.getName().toUri();
  put(data);
  m_manager->addIdentity(keyData.getName(), cert);
}

bool
Service::onRemoveIdentity(const Interest& interest,
                          const Name& args,
                          shared_ptr<Data> data,
                          InterestShower want,
                          PutData put)
{
  Name identity{args};
  authenticateManagementInterest(interest, identity);
  m_manager->removeIdentity(identity);
  return false;
}

bool
Service::onGrant(const Interest& interest,
                 const Name& args,
                 shared_ptr<Data> data,
                 InterestShower want,
                 PutData put)
{
  string identity = strings::uriDecode(args.get(0).toUri());
  string dataType = strings::uriDecode(args.get(1).toUri());

  // string grantType = args.get(2).toUri(); // reserved
  // string startDate = args.get(3).toUri();
  // string endDate = args.get(4).toUri();
  // string startHour = args.get(5).toUri();
  // string endHour = args.get(6).toUri();

  string startDate = "20170703T000000Z";
  string endDate = "20170730T000000Z";
  string startHour = "00";
  string endHour = "23";

  LOG(INFO) << std::endl
            << "= = = = G R A N T = = = = " << std::endl
            << "identity: " << identity << std::endl
            << "dataType: " << dataType << std::endl
            << "start: " << startDate << std::endl
            << "end: " << endDate << std::endl
            << "- - - - - - - - - - - - - ";

  grant(identity, dataType, startDate, endDate, startHour, endHour);
  return false;
}

bool
Service::onRevoke(const Interest& interest,
                  const Name& args,
                  shared_ptr<Data> data,
                  InterestShower want,
                  PutData put)
{
  if (args.size() < 2) {
    throw "onRevoke: argument should contains at least 2 components, got :" +
      std::to_string(args.size());
  }

  string identity = strings::uriDecode(args.get(0).toUri());
  string dataType = strings::uriDecode(args.get(1).toUri());
  m_manager->revokeAccess(identity, dataType);
  return false;
}

void
Service::grant(const Name& identity,
               const Name& dataType,
               string startDate,
               string endDate,
               string startHour,
               string endHour)
{
  TimeStamp startD = boost::posix_time::from_iso_string(startDate);
  TimeStamp endD = boost::posix_time::from_iso_string(endDate);
  int startH = std::stoi(startHour);
  int endH = std::stoi(endHour);
  // auto days = (endD.date() - startD.date()).days();
  RepetitiveInterval schedule(startD, endD, startH, endH, 1, RepetitiveInterval::RepeatUnit::DAY);

  LOG(INFO) << "[DEBUG] Created schedule " << schedule.getStartDate() << " - "
            << schedule.getEndDate() << " (" << schedule.getIntervalStartHour() << " ~ "
            << schedule.getIntervalEndHour() << ") * " << schedule.getNRepeats();
  const string schedule_name{dataType.toUri() + startDate + endDate + startHour + endHour};
  NamedInterval ni(schedule_name, schedule);
  m_manager->grantAccess(identity, dataType, ni);
}

Buffer
Service::parseIdentityPubKey(const Data& keyData)
{
  Certificate selfCert(keyData);
  return selfCert.getPublicKey();
}

Certificate
Service::signPubkey(const Name& name, const Buffer& key)
{
  auto keyPtr = make_shared<const Buffer>(key);
  Data d;
  d.setName(name);
  d.setContent(keyPtr);
  d.setContentType(ndn::tlv::ContentType_Key);
  d.setFreshnessPeriod(time::milliseconds(60000)); //TODO: use parameter instead.
  Certificate c(d);
  m_keychain->sign(c);
  return c;
}

void
Service::authenticateManagementInterest(const Interest& interest, const Name entity)
{
  // TODO;
}

} // nacapp