#ifndef NODE_HPP
#define NODE_HPP

#include "common.hpp"
#include "handlers.hpp"
#include "node-impl.hpp"

namespace nacapp {

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
  Node(Name prefix, shared_ptr<Face> f) : impl(prefix, f) {}

public:
  /**
   * Start serving.
   */
  void serveForever();

  /**
   * setInterestFilter for <m_prefix><path>
   * Parts after <path> and before SIGNATURE is <ARGUMETNS
   * TODO: make handler more generic. (maybe use some templating tricks)
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
  NodeImpl impl;
};

} // nacapp
#endif /* NODE_HPP */