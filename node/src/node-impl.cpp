#include "node-impl.hpp"

namespace nacapp {

void
NodeImpl::serveForever()
{
  LOG(INFO) << "node init...";
  LOG(INFO) << m_handlers.size() << " interest handlers registered";
  LOG(INFO) << m_commonValidators.size() << " interest validators registered ";
  LOG(INFO) << m_commonProcessors.size() << " data processors registered";
  LOG(INFO) << "start registering prefixes";
  registerPrefixes();
  LOG(INFO) << "start serving...";
  while (true) {
    m_face->processEvents();
  }
  LOG(ERROR) << "server quit.";
}

void
NodeImpl::route(string path,
                InterestHandler handler,
                vector<InterestValidator> validators,
                vector<DataProcessor> processors)
{
  LOG(INFO) << "register: " << path << " [ " << validators.size() << " " << processors.size() << " ]";
  m_handlers[path] = handler;
  if (validators.size() > 0) {
    m_validators[path] = validators;
  }
  if (processors.size() > 0) {
    m_processors[path] = processors;
  }
}

void
NodeImpl::registerPrefixes()
{
  for (auto item : m_handlers) {
    const Name path = item.first;
    InterestHandler handler = item.second;
    Name prefix(m_prefix);
    if (path.toUri() != "/") {
      prefix.append(path);
    }
    LOG(INFO) << "register prefix : " << prefix.toUri();
    m_face->setInterestFilter(prefix,
                              bind(&NodeImpl::onInterest, this, _1, _2),
                              ndn::RegisterPrefixSuccessCallback(),
                              bind(&NodeImpl::onRegisterPrefixFailed, this, _1, _2));
  }
}

void
NodeImpl::onInterest(const Name& filter, const Interest& interest)
{
  string err;
  bool isError = true;
  try {
    LOG(INFO) << "In: " << interest.toUri();
    vector<Name> parts = parseInterestName(interest);
    handleInterest(interest, parts);
    isError = false;
  }
  catch (string e) {
    err = e;
  }
  catch (const std::exception& exc) {
    err = exc.what();
  }
  catch (...) {
    err = "unknown error";
  }
  if (isError) {
    LOG(ERROR) << "Error handling " << interest.toUri() << ": " << err;
    onFailed(interest, err);
  }
}

void
NodeImpl::setPrefix(Name prefix)
{
  m_prefix = prefix;
}

void
NodeImpl::onRegisterPrefixFailed(const Name& prefix, const std::string& reason)
{
  LOG(ERROR) << "cannot register prefix[" << prefix.toUri() << "]: " << reason;
  throw std::runtime_error("prefix registeration error");
}

void
NodeImpl::showInterest(const Interest& interest, DataReceiver proc)
{
  m_face->expressInterest(interest,
                          // on satisfied
                          [&](const Interest& i, const Data& d) { proc(d); },
                          // on NACK
                          [&](const Interest& i, const ndn::lp::Nack& n) {
                            LOG(ERROR) << i.toUri() << ": " << n.getReason();
                            const string reason = "network error";
                            Data d;
                            d.setName(i.getName());
                            d.setContentType(ndn::tlv::ContentType_Nack);
                            data::setStringContent(d, reason);
                            auto dataPtr = make_shared<Data>(d);
                            for (DataProcessor processor : m_commonProcessors) {
                              processor(dataPtr);
                            }
                            proc(d);
                          },
                          // on timeout
                          [&](const Interest& i) {
                            LOG(ERROR) << i.toUri() << ": TIMEOUT";
                            const string reason = "interest timeout";
                            Data d;
                            d.setName(i.getName());
                            d.setContentType(ndn::tlv::ContentType_Nack);
                            data::setStringContent(d, reason);
                            auto dataPtr = make_shared<Data>(d);
                            for (DataProcessor processor : m_commonProcessors) {
                              processor(dataPtr);
                            }
                            proc(d);
                          });
}

void
NodeImpl::validate(const Name& path, const Interest& interest)
{
  for (InterestValidator validator : m_commonValidators) {
    validator(interest);
  }
  if (m_validators.find(path) != m_validators.end()) {
    for (InterestValidator validator : m_validators[path]) {
      validator(interest);
    }
  }
}

void
NodeImpl::process(const Name& path, const Interest& interest, shared_ptr<Data> data)
{
  for (DataProcessor processor : m_commonProcessors) {
    processor(data);
  }
  if (m_processors.find(path) != m_processors.end()) {
    for (DataProcessor processor : m_processors[path]) {
      processor(data);
    }
  }
}

shared_ptr<Data>
NodeImpl::handleInterest(const Interest& interest, vector<Name> parsedParts)
{
  Name prefix = parsedParts[0];
  Name path = parsedParts[1];
  Name args = parsedParts[2];
  LOG(INFO) << std::endl
            << "\tPrefix: " << prefix << std::endl
            << "\tPath: " << path << std::endl
            << "\tArgs:" << args;

  validate(path, interest);

  if (m_handlers.find(path) == m_handlers.end()) {
    throw "Cannot find handler for interest: " + interest.getName().toUri();
  }

  InterestHandler handler = m_handlers[path];
  auto data = make_shared<Data>();
  data->setName(interest.getName());
  InterestShower show = std::bind(&NodeImpl::showInterest, this, _1, _2);
  shared_ptr<bool> sent = make_shared<bool>(false);
  PutData put = std::bind(&NodeImpl::sendData, this, path, interest, sent, _1);

  bool async = handler(interest, args, data, show, put);
  if (!async && !sent) {
    put(data);
  }
  return data;
}

void
NodeImpl::onFailed(const Interest& interest, string reason)
{
  auto data = make_shared<Data>();
  data->setContentType(ndn::tlv::ContentType_Nack);
  data::setStringContent(*data, reason);
  m_keychain->sign(*data);
  m_face->put(*data);
}

void
sendNack(shared_ptr<Face> face, const Interest& interest)
{
  // TODO: does not support customized reason?
  // ref: https://github.com/named-data/ndn-cxx/blob/master/src/lp/nack-header.hpp#L39
  ndn::lp::Nack nack(interest);
  face->put(nack);
}


void
NodeImpl::sendData(const Name& path,
                   const Interest& interest,
                   shared_ptr<bool> sent,
                   shared_ptr<Data> data)
{
  // NAC process data with nack type as normal data and crashes the application.
  // So we need to convert all nack data into lp::Nack here.
  if (data->getContentType() == ndn::tlv::ContentType_Nack) {
    LOG(INFO) << "App Nack: " << interest.toUri();
    sendNack(m_face, interest);
    return;
  }

  // apply post processors
  process(path, interest, data);
  // sign the data if it is not signed yet .
  if (!data->getSignature()) {
    m_keychain->sign(*data);
  }
  if (data->getFreshnessPeriod() == time::milliseconds::zero()) {
    data->setFreshnessPeriod(time::milliseconds(1000));
  }
  m_face->put(*data);
  *sent = true;
  LOG(INFO) << "[out] Data: " << (*data).getName().toUri();
  LOG(INFO) << "\tSize: " << (*data).getContent().size();
}

vector<Name>
NodeImpl::parseInterestName(const Interest& interest)
{
  vector<Name> v;

  // extract prefix
  size_t offset = m_prefix.size();
  const Name iname = interest.getName();
  if (iname.size() < offset) {
    throw "interest name is less than prefix";
  }
  Name prefix = iname.getPrefix(offset);
  if (prefix != m_prefix) {
    throw "prefix does not match";
  }
  v.push_back(prefix);


  // extract path
  bool found = false;
  for (auto entry : m_handlers) {
    Name p = entry.first;
    if (p.toUri() == NODE_DEFAULT_PATH) {
      continue;
    }
    Name path = iname.getSubName(offset, p.size());
    // LOG(INFO) << "[D] chech path: " << std::endl << "\t" << p << std::endl << "\t" << path;
    if (p == path) {
      found = true;
      offset += p.size();
      v.push_back(path);
      break;
    }
  }
  if (!found) {
    LOG(WARNING) << "didn't find registered path for interest : " << interest.toUri()
                 << ". Using default handler /";
    v.push_back(NODE_DEFAULT_PATH);
  }

  // according to
  //   http://named-data.net/doc/ndn-cxx/current/tutorials/signed-interest.html
  // the following value should be 4 instead of 2.
  // But in test we are only getting 2 instead of 4.
  // const size_t SIGNATURE_COMPONENTS = 2;
  // const size_t n = iname.size() - offset - SIGNATURE_COMPONENTS;
  const size_t n = iname.size() - offset;
  Name args;
  if (n > 0) {
    args = iname.getSubName(offset, n);
  }
  else {
    args = Name("");
  }
  v.push_back(args);
  return v;
}

} // nacapp