#include "node.hpp"
namespace nacapp {

void Node::serveForever() {
  LOG(DEBUG) << "start serving...";
  LOG(DEBUG) << m_handlers.size() << " interest handlers registered";
  LOG(DEBUG) << m_commonValidators.size() << " interest validators registered";
  LOG(DEBUG) << m_commonProcessors.size() << " data processors registered";
  LOG(DEBUG) << "start registering prefixes";
  registerPrefixes();
  LOG(DEBUG) << "start serving...";
  m_face->processEvents();
  LOG(ERROR) << "server quit.";
}

void Node::registerPrefixes() {
  m_face->setInterestFilter(m_prefix, bind(&Node::onInterest, this, _1, _2),
                            ndn::RegisterPrefixSuccessCallback(),
                            bind(&Node::onRegisterPrefixFailed, this, _1, _2));
}

void Node::onRegisterPrefixFailed(const Name &prefix,
                                  const std::string &reason) {
  LOG(ERROR) << "cannot register prefix[" << prefix.toUri() << "]: " << reason;
  throw std::runtime_error("prefix registeration error");
}

void Node::onInterest(const Name &filter, const Interest &interest) {
  try {
    LOG(DEBUG) << "In: " << interest.toUri();
    vector<Name> parts = parseInterestName(interest);
    shared_ptr<Data> data = handleInterest(interest, parts);
  } catch (string e) {
    LOG(ERROR) << "Error handling " << interest.toUri() << ": " << e;
    onFailed(interest, e);
    return;
  }
}

void Node::validate(const Name &path, const Interest &interest) {
  for (InterestValidator validator : m_commonValidators) {
    validator(interest);
  }
  if (m_validators.find(path) != m_validators.end()) {
    for (InterestValidator validator : m_validators[path]) {
      validator(interest);
    }
  }
}

void Node::process(const Name &path, const Interest &interest,
                   shared_ptr<Data> data) {
  for (DataProcessor processor : m_commonProcessors) {
    processor(data);
  }
  if (m_processors.find(path) != m_processors.end()) {
    for (DataProcessor processor : m_processors[path]) {
      processor(data);
    }
  }
}

shared_ptr<Data> Node::handleInterest(const Interest &interest,
                                      vector<Name> parsedParts) {

  Name prefix = parsedParts[0];
  Name path = parsedParts[1];
  Name args = parsedParts[2];

  validate(path, interest);

  if (m_handlers.find(path) == m_handlers.end()) {
    throw "Cannot find handler for interest: " + interest.getName().toUri();
  }
  InterestHandler handler = m_handlers[path];
  auto data = make_shared<Data>();
  handler(path, args, data);

  process(path, interest, data);

  return data;
}

void Node::onFailed(const Interest &interest, string reason) {
  auto data = make_shared<Data>();
  data->setContentType(ndn::tlv::ContentType_Nack);
  data->setContent(reinterpret_cast<const uint8_t *>(reason.c_str()),
                   reason.length());
  sendData(interest, data);
}

vector<Name> Node::parseInterestName(const Interest &interest) {
  vector<Name> v;
  throw "Node::parseInterestName not implemented";
  return v;
}

} // nacapp