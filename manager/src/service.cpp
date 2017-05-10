#include "service.hpp"

namespace nacapp
{

void Service::onGetEKey(const Interest &interest, const Name args,
                        shared_ptr<Data> data, InterestShower want)
{

  if (args.size() < 2)
  {
    throw "onGetEKey requires at least 2 arguments, got : " + args.toUri();
  }

  const Name dataType{interest.getName().at(m_prefix.size() + 1)};
  const Name entity{args.at(0)};
  const string timestamp{args.at(1).toUri()};
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timestamp);
  shared_ptr<Data> ekey = m_manager.getEKey(dataType, timeslot);
  data->setContent(ekey->wireEncode());
}

void Service::onGetDKey(const Interest &interest, const Name args,
                        shared_ptr<Data> data, InterestShower want)
{
  const Name dataType{interest.getName().at(m_prefix.size() + 1)};
  const Name entity{args.at(0)};
  const string timestamp{args.at(1).toUri()};
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timestamp);
  shared_ptr<Data> dkey = m_manager.getDKey(entity, dataType, timeslot);
  data->setContent(dkey->wireEncode());
}

void Service::onGetIdentityKey(const Interest &interest, const Name args,
                               shared_ptr<Data> data, InterestShower want)
{
  Name entity{args};
  shared_ptr<Certificate> id = m_manager.getIdentity(entity);
  data->setContent(id->wireEncode());
}

void Service::onAddIdentity(const Interest &interest, const Name args,
                            shared_ptr<Data> data, InterestShower want)
{
  Name entity{args};
  authenticateAddIdentityInterest(interest, entity);
  const ndn::KeyLocator keyLocator = interest.getPublisherPublicKeyLocator();
  if (keyLocator.getType() == ndn::KeyLocator::KeyLocator_Name)
  {
    auto keyName = keyLocator.getName();
    Interest identityKey(keyName);
    want(identityKey, [&](const Data &keyData) {
      Buffer pubkey = this->parseIdentityPubKey(keyData);
      Certificate cert = this->signPubkey(pubkey);
      this->m_manager.addIdentity(entity, cert);
    });
  }
  else
  {
    throw "Add identity: interest KeyLocator must contain the name of the key";
  }
}

void Service::onRemoveIdentity(const Interest &interest, const Name args,
                               shared_ptr<Data> data, InterestShower want) {}

void Service::onGrantFixed(const Interest &interest, const Name args,
                           shared_ptr<Data> data, InterestShower want) {}

void Service::onRevoke(const Interest &interest, const Name args,
                       shared_ptr<Data> data, InterestShower want) {}

Buffer Service::parseIdentityPubKey(const Data &keyData)
{
  Certificate selfCert(keyData);
  return selfCert.getPublicKey();
}

Certificate Service::signPubkey(const Buffer &key)
{
  auto keyPtr = make_shared<const Buffer>(key);
  Block block(keyPtr, keyPtr->begin(), keyPtr->end());
  Data d(block);
  Certificate c(d);
  AppKeyChain.sign(c);
  return c;
}

void Service::authenticateAddIdentityInterest(const Interest &interest, const Name entity)
{
  // TODO;
}

} // nacapp