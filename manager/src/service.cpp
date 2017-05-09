#include "service.hpp"

namespace nacapp
{

void Service::onGetEKey(const Interest &interest, const Name args,
                        shared_ptr<Data> data)
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
                        shared_ptr<Data> data)
{
  const Name dataType{interest.getName().at(m_prefix.size() + 1)};
  const Name entity{args.at(0)};
  const string timestamp{args.at(1).toUri()};
  const TimeStamp timeslot = boost::posix_time::from_iso_string(timestamp);
  shared_ptr<Data> dkey = m_manager.getDKey(entity, dataType, timeslot);
  data->setContent(dkey->wireEncode());
}

void Service::onGetIdentityKey(const Interest &interest, const Name args,
                               shared_ptr<Data> data)
{
  Name entity{args};
  shared_ptr<Certificate> id = m_manager.getIdentity(entity);
  data->setContent(id->wireEncode());
}

void Service::onAddIdentity(const Interest &interest, const Name args,
                            shared_ptr<Data> data)
{
  Name entity{args};
  const ndn::KeyLocator keyLocator = interest.getPublisherPublicKeyLocator();
  if (keyLocator.getType() == ndn::KeyLocator::KeyLocator_Name)
  {
  }
  else
  {
    throw "Add identity: keyLocator must be a name of the key";
  }
}

void Service::onRemoveIdentity(const Interest &interest, const Name args,
                               shared_ptr<Data> data) {}

void Service::onGrantFixed(const Interest &interest, const Name args,
                           shared_ptr<Data> data) {}

void Service::onRevoke(const Interest &interest, const Name args,
                       shared_ptr<Data> data) {}

} // nacapp