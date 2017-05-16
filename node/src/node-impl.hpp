#ifndef NODE_IMPL_HPP
#define NODE_IMPL_HPP

#include <ndn-cxx/lp/nack.hpp>

#include "common.hpp"
#include "handlers.hpp"
#include "../../shared/src/ndn-util.hpp"

namespace nacapp {

class NodeImpl
{
public:
  NodeImpl(Name prefix, shared_ptr<Face> f)
    : m_prefix(prefix)
    , m_face(f)
  {
  }

public:
  void
  serveForever();
  void
  route(string path, InterestHandler handler, vector<InterestValidator> validators,
        vector<DataProcessor> processors);

public:
  void
  registerPrefixes();

  void
  onRegisterPrefixFailed(const Name& prefix, const std::string& reason);

  void
  onInterest(const Name& filter, const Interest& interest);

  void
  onFailed(const Interest& interest, string reason);

  // TODO : Should handle packet segmentation as Data packets have size limit.
  void
  sendData(const Interest& interest, shared_ptr<Data> data);

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
  map<Name, InterestHandler> m_handlers;
  vector<InterestValidator> m_commonValidators;
  vector<DataProcessor> m_commonProcessors;
  map<Name, vector<InterestValidator>> m_validators;
  map<Name, vector<DataProcessor>> m_processors;
};

} // nacapp
#endif /* NODE_IMPL_HPP */
