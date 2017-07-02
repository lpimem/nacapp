#ifndef MANAGER_SERVICE_NODE_HPP
#define MANAGER_SERVICE_NODE_HPP

#include "../../node/src/node.hpp"

#include "common.hpp"

#include "service.hpp"

namespace nacapp {
class ManagerServiceNode
{
public:
  ManagerServiceNode(Name prefix, shared_ptr<Face> f, shared_ptr<KeyChain> kc)
    : m_prefix(prefix)
    , m_node(prefix, f, kc)
    , m_service(prefix, kc)
  {
  }

public:
  void
  serveForever();

  const Name&
  getName()
  {
    return m_prefix;
  }

  const Node&
  getNode()
  {
    return m_node;
  }

protected:
  void
  addRoutes();

private:
  Name m_prefix;
  Node m_node;
  Service m_service;
};
} // nacapp
#endif /* MANAGER_SERVICE_NODE_HPP */
