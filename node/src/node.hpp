#ifndef NODE_HPP
#define NODE_HPP

#include "common.hpp"

namespace nacapp {

// application interest handlers. @p path is the name components after the
// application namespace. @p args is the name components after @p path but
// before the signature components.
using InterestHandler = void (*)(const Name &path, const Name &args,
                                 shared_ptr<Data> data);

// for common validation functions. They are called before interest handlers, if
// something is wrong, the validator should throw a string explaining the error.
using InterestValidator = void (*)(const Interest &);

// for common post-processing functions. They are called after the interest
// handlers.
using DataProcessor = void (*)(shared_ptr<Data>);

/**
 * Handles interests with name in the following format
 *    NAME ::= PREFIX/PATH/ARGUMETNS/SIGNATURE
 *    PREFIX ::= NameComponent+
 *    ARGUMETNS ::= NameComponent*
 *    SIGNATURE ::= NameComponent{4}
 *
 * Reference:
 *    NameComponent: http://named-data.net/doc/NDN-TLV/current/name.html
 */
class Node {
public:
  Node(Name prefix, shared_ptr<Face> f) : m_prefix(prefix), m_face(f) {}

public:
  /**
   * Start serving.
   */
  void serveForever();

  /**
   * setInterestFilter for <m_prefix><path>
   * Parts after <path> and before SIGNATURE is <ARGUMETNS
   * TODO: make handler more generic. (maybe can use some templating tricks)
   */
  void route(string path, InterestHandler handler);

  void route(string path, InterestHandler handler,
             vector<InterestValidator> validators);

  void route(string path, InterestHandler handler,
             vector<DataProcessor> processors);

  void route(string path, InterestHandler handler,
             vector<InterestValidator> validators,
             vector<DataProcessor> processors);

private:
  void registerPrefixes();

  void onRegisterPrefixFailed(const Name &prefix, const std::string &reason);

  void onInterest(const Name &filter, const Interest &interest);

  void onFailed(const Interest &interest, string reason);

  void sendData(const Interest &interest, shared_ptr<Data> data);

  void validate(const Name &path, const Interest &interest);

  void process(const Name &path, const Interest &interest,
               shared_ptr<Data> data);

  vector<Name> parseInterestName(const Interest &interest);

  shared_ptr<Data> handleInterest(const Interest &interest,
                                  const vector<Name> parsedParts);

private:
  Name m_prefix;
  shared_ptr<Face> m_face;
  map<Name, InterestHandler> m_handlers;
  vector<InterestValidator> m_commonValidators;
  vector<DataProcessor> m_commonProcessors;
  map<Name, vector<InterestValidator>> m_validators;
  map<Name, vector<DataProcessor>> m_processors;
};
}
#endif /* NODE_HPP */
