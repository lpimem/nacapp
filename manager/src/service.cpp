#include "service.hpp"
#include "util.hpp"

namespace nacapp {

void
Service::onGetEKey(const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower want,
                   PutData put)
{
  if (args.size() < 2) {
    throw "onGetEKey requires at least 2 arguments, got : " + args.toUri();
  }

  const Name dataType{interest.getName().at(m_prefix.size() + 1)};
  const Name entity{args.at(0)};
  const string timestamp{args.at(1).toUri()};
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timestamp);
  shared_ptr<Data> ekey = m_manager.getEKey(dataType, timeslot);
  data->setContent(ekey->wireEncode());
}

void
Service::onGetDKey(const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower want,
                   PutData put)
{
  const Name dataType{interest.getName().at(m_prefix.size() + 1)};
  const Name entity{args.at(0)};
  const string timestamp{args.at(1).toUri()};
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timestamp);
  shared_ptr<Data> dkey = m_manager.getDKey(entity, dataType, timeslot);
  data->setContent(dkey->wireEncode());
}

void
Service::onGetIdentityKey(const Interest& interest,
                          const Name& args,
                          shared_ptr<Data> data,
                          InterestShower want,
                          PutData put)
{
  Name entity{args};
  shared_ptr<Certificate> id = m_manager.getIdentity(entity);
  data->setContent(id->wireEncode());
}

void
Service::onAddIdentity(const Interest& interest,
                       const Name& args,
                       shared_ptr<Data> data,
                       InterestShower want,
                       PutData put)
{
  Name entity{args};
  authenticateManagementInterest(interest, entity);
  const ndn::KeyLocator keyLocator = interest.getPublisherPublicKeyLocator();
  if (keyLocator.getType() == ndn::KeyLocator::KeyLocator_Name) {
    auto keyName = keyLocator.getName();
    Interest identityKey(keyName);
    want(identityKey, [&](const Data& keyData) {
      Buffer pubkey = this->parseIdentityPubKey(keyData);
      Certificate cert = this->signPubkey(pubkey);
      this->m_manager.addIdentity(entity, cert);
    });
  }
  else {
    throw "Add identity: interest KeyLocator must contain the name of the key";
  }
}

void
Service::onRemoveIdentity(const Interest& interest,
                          const Name& args,
                          shared_ptr<Data> data,
                          InterestShower want,
                          PutData put)
{
  Name identity{args};
  authenticateManagementInterest(interest, identity);
  m_manager.removeIdentity(identity);
}

void
Service::onGrant(const Interest& interest,
                 const Name& args,
                 shared_ptr<Data> data,
                 InterestShower want,
                 PutData put)
{
  if (args.size() < 7) {
    throw "onGrant: argument should contains at least 7 components, got :" +
      std::to_string(args.size());
  }

  string identity = strings::uriDecode(args.get(0).toUri());
  string dataType = strings::uriDecode(args.get(1).toUri());
  string grantType = args.get(2).toUri(); // reserved
  string startDate = args.get(3).toUri();
  string endDate = args.get(4).toUri();
  string startHour = args.get(5).toUri();
  string endHour = args.get(6).toUri();
  grant(identity, dataType, startDate, endDate, startHour, endHour);
}

void
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
  m_manager.revokeAccess(identity, dataType);
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
  auto days = (endD.date() - startD.date()).days();
  RepetitiveInterval schedule(startD, endD, startH, endH, days, RepetitiveInterval::RepeatUnit::DAY);
  const string schedule_name{dataType.toUri() + startDate + endDate + startHour + endHour};
  NamedInterval ni(schedule_name, schedule);
}

Buffer
Service::parseIdentityPubKey(const Data& keyData)
{
  Certificate selfCert(keyData);
  return selfCert.getPublicKey();
}

Certificate
Service::signPubkey(const Buffer& key)
{
  auto keyPtr = make_shared<const Buffer>(key);
  Block block(keyPtr, keyPtr->begin(), keyPtr->end());
  Data d(block);
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