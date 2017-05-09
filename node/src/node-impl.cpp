#include "node-impl.hpp"

namespace nacapp
{

void NodeImpl::serveForever()
{
  LOG(DEBUG) << "start serving...";
  LOG(DEBUG) << m_handlers.size() << " interest handlers registered";
  LOG(DEBUG) << m_commonValidators.size() << " interest validators registered ";
  LOG(DEBUG) << m_commonProcessors.size() << " data processors registered";
  LOG(DEBUG) << "start registering prefixes";
  registerPrefixes();
  LOG(DEBUG) << "start serving...";
  m_face->processEvents();
  LOG(ERROR) << "server quit.";
}

void NodeImpl::route(string path, InterestHandler handler,
                     vector<InterestValidator> validators,
                     vector<DataProcessor> processors)
{
  m_handlers[path] = handler;
  if (validators.size() > 0)
  {
    m_validators[path] = validators;
  }
  if (processors.size() > 0)
  {
    m_processors[path] = processors;
  }
}

void NodeImpl::registerPrefixes()
{
  m_face->setInterestFilter(
      m_prefix, bind(&NodeImpl::onInterest, this, _1, _2),
      ndn::RegisterPrefixSuccessCallback(),
      bind(&NodeImpl::onRegisterPrefixFailed, this, _1, _2));
}

void NodeImpl::onRegisterPrefixFailed(const Name &prefix,
                                      const std::string &reason)
{
  LOG(ERROR) << "cannot register prefix[" << prefix.toUri() << "]: " << reason;
  throw std::runtime_error("prefix registeration error");
}

void NodeImpl::showInterest(const Interest &interest, DataReceiver proc)
{
  m_face->expressInterest(interest,
                          // on satisfied
                          [&](const Interest &i, const Data &d) {
                            proc(d);
                          },
                          // on NACK
                          [&](const Interest &i, const ndn::lp::Nack &n) {
                            LOG(ERROR) << i.toUri() << ": " << n.getReason();
                            const string reason = "network error";
                            Data d;
                            d.setName(i.getName());
                            d.setContentType(ndn::tlv::ContentType_Nack);
                            data::setStringContent(d, reason);
                            auto dataPtr = make_shared<Data>(d);
                            for (DataProcessor processor : m_commonProcessors)
                            {
                              processor(dataPtr);
                            }
                            proc(d);
                          },
                          // on timeout
                          [&](const Interest &i) {
                            LOG(ERROR) << i.toUri() << ": TIMEOUT";
                            const string reason = "interest timeout";
                            Data d;
                            d.setName(i.getName());
                            d.setContentType(ndn::tlv::ContentType_Nack);
                            data::setStringContent(d, reason);
                            auto dataPtr = make_shared<Data>(d);
                            for (DataProcessor processor : m_commonProcessors)
                            {
                              processor(dataPtr);
                            }
                            proc(d);
                          });
}

void NodeImpl::onInterest(const Name &filter, const Interest &interest)
{
  try
  {
    LOG(DEBUG) << "In: " << interest.toUri();
    vector<Name> parts = parseInterestName(interest);
    shared_ptr<Data> data = handleInterest(interest, parts);
  }
  catch (string e)
  {
    LOG(ERROR) << "Error handling " << interest.toUri() << ": " << e;
    onFailed(interest, e);
    return;
  }
}

void NodeImpl::validate(const Name &path, const Interest &interest)
{
  for (InterestValidator validator : m_commonValidators)
  {
    validator(interest);
  }
  if (m_validators.find(path) != m_validators.end())
  {
    for (InterestValidator validator : m_validators[path])
    {
      validator(interest);
    }
  }
}

void NodeImpl::process(const Name &path, const Interest &interest,
                       shared_ptr<Data> data)
{
  for (DataProcessor processor : m_commonProcessors)
  {
    processor(data);
  }
  if (m_processors.find(path) != m_processors.end())
  {
    for (DataProcessor processor : m_processors[path])
    {
      processor(data);
    }
  }
}

shared_ptr<Data> NodeImpl::handleInterest(const Interest &interest,
                                          vector<Name> parsedParts)
{
  Name prefix = parsedParts[0];
  Name path = parsedParts[1];
  Name args = parsedParts[2];

  validate(path, interest);

  if (m_handlers.find(path) == m_handlers.end())
  {
    throw "Cannot find handler for interest: " + interest.getName().toUri();
  }
  InterestHandler handler = m_handlers[path];
  auto data = make_shared<Data>();
  InterestShower show = std::bind(&NodeImpl::showInterest, this, _1, _2);
  handler(interest, args, data, show);
  process(path, interest, data);
  return data;
}

void NodeImpl::onFailed(const Interest &interest, string reason)
{
  auto data = make_shared<Data>();
  data->setContentType(ndn::tlv::ContentType_Nack);
  data::setStringContent(*data, reason);
  // process()
  sendData(interest, data);
}

void NodeImpl::sendData(const Interest &interest, shared_ptr<Data> data)
{
  m_face->put(*data);
}

vector<Name> NodeImpl::parseInterestName(const Interest &interest)
{
  vector<Name> v;
  size_t offset = m_prefix.size();
  const Name iname = interest.getName();
  Name prefix = iname.getPrefix(offset);
  if (prefix != m_prefix)
  {
    throw "prefix does not match";
  }
  v.push_back(prefix);

  bool found = false;
  for (auto entry : m_handlers)
  {
    Name p = entry.first;
    Name path = iname.getSubName(offset, p.size());
    if (p == path)
    {
      found = true;
      offset += p.size();
      v.push_back(path);
      break;
    }
  }
  if (!found)
  {
    throw "no matching handler found";
  }

  // according to
  //   http://named-data.net/doc/ndn-cxx/current/tutorials/signed-interest.html
  // the following value should be 4 instead of 2.
  // But in test we are only getting 2 instead of 4.
  const size_t SIGNATURE_COMPONENTS = 2;

  Name args =
      iname.getSubName(offset, iname.size() - offset - SIGNATURE_COMPONENTS);
  v.push_back(args);

  return v;
}

} // nacapp