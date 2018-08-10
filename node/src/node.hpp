#ifndef NODE_HPP
#define NODE_HPP

#include "common.hpp"
#include "handlers.hpp"
#include "node-impl.hpp"

namespace nacapp {

/**
 * Handles interests with name in the following format
 *    NAME          ::= PREFIX/PATH/ARGUMETNS/SIGNATURE
 *    PREFIX|PATH   ::= NameComponent+
 *    ARGUMETNS     ::= NameComponent*
 *    SIGNATURE     ::= NameComponent{4}   
 *    // a signature has 4 components as documented, but only 2 in acutal observation.
 *    // what did I miss?
 *
 * Reference:
 *    NameComponent: http://named-data.net/doc/NDN-TLV/current/name.html
 */
class Node
{
public:
  Node(Name prefix, shared_ptr<Face> f, shared_ptr<KeyChain> keychain)
    : impl(prefix, f, keychain)
  {
  }

public:
  void
  setPrefix(Name prefix)
  {
    impl.setPrefix(prefix);
  }

  /**
   * Start serving.
   */
  void
  serveForever();

  /**
   * setInterestFilter for <m_prefix><path>
   * Parts after <path> and before SIGNATURE is <ARGUMETNS
   * TODO: make handler more generic and type-safe. (maybe use some templating tricks)
   */
  void
  route(string path, InterestHandler handler);

  void
  route(string path, InterestHandler handler, vector<InterestValidator> validators);

  void
  route(string path, InterestHandler handler, vector<DataProcessor> processors);

  void
  route(string path,
        InterestHandler handler,
        vector<InterestValidator> validators,
        vector<DataProcessor> processors);

  void
  setTrustAnchor(Certificate&&);

  void
  showInterest(const Interest&, DataReceiver);

private:
  NodeImpl impl;
};

} // namespace nacapp
#endif /* NODE_HPP */
