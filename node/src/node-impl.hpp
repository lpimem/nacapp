#ifndef NODE_IMPL_HPP
#define NODE_IMPL_HPP

#include <ndn-cxx/lp/nack.hpp>
#include <ndn-cxx/security/v2/certificate-fetcher-from-network.hpp>
#include <ndn-cxx/security/v2/validation-policy-simple-hierarchy.hpp>
#include <ndn-cxx/security/v2/validator.hpp>

#include "common.hpp"
#include "handlers.hpp"
#include "../../shared/src/ndn-util.hpp"

namespace nacapp {

class NodeImpl
{
public:
  NodeImpl(Name prefix, shared_ptr<Face> f, shared_ptr<KeyChain> keychain)
    : m_prefix(prefix)
    , m_face(f)
    , m_keychain(keychain)
    , m_ndn_validator(make_unique<ndn::security::v2::ValidationPolicySimpleHierarchy>(),
                      make_unique<ndn::security::v2::CertificateFetcherFromNetwork>(*f))
  {
  }

public:
  void
  serveForever();
  void
  route(string path,
        InterestHandler handler,
        vector<InterestValidator> validators,
        vector<DataProcessor> processors);

public:
  void
  registerPrefixes();

  void
  setPrefix(Name prefix);

  void
  setKeychain(shared_ptr<KeyChain> kc)
  {
    m_keychain = kc;
  }

  void
  setTrustAnchor(Certificate& ta);

  void
  onRegisterPrefixFailed(const Name& prefix, const std::string& reason);

  void
  onInterest(const Name& filter, const Interest& interest);

  void
  onFailed(const Interest& interest, string reason);

  void
  sendData(const Name& path, const Interest& interest, shared_ptr<bool> sent, shared_ptr<Data> data);

  void
  validate(const Name& path, const Interest& interest);

  void
  process(const Name& path, const Interest& interest, shared_ptr<Data> data);

  vector<Name>
  parseInterestName(const Interest& interest);

  shared_ptr<Data>
  handleInterest(const Interest& interest, vector<Name> parsedParts);

  void
  showInterest(const Interest& interest, DataReceiver proc);

private:
  Name m_prefix;
  shared_ptr<Face> m_face;
  shared_ptr<KeyChain> m_keychain;
  map<Name, InterestHandler> m_handlers;
  vector<InterestValidator> m_commonValidators;
  vector<DataProcessor> m_commonProcessors;
  ndn::security::v2::Validator m_ndn_validator;
  map<Name, vector<InterestValidator>> m_validators;
  map<Name, vector<DataProcessor>> m_processors;
};

} // namespace nacapp
#endif /* NODE_IMPL_HPP */
