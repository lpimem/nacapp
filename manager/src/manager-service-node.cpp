#include "manager-service-node.hpp"


namespace nacapp {

void
ManagerServiceNode::addRoutes()
{
  m_node.route("/READ", std::bind(&Service::onRead, m_service, _1, _2, _3, _4, _5));

  m_node.route("/IDENTITY/for", std::bind(&Service::onGetIdentityKey, m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/identity/add",
               std::bind(&Service::onAddIdentity, m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/identity/remove",
               std::bind(&Service::onRemoveIdentity, m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/access/grant",
               std::bind(&Service::onGrant, m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/access/revoke",
               std::bind(&Service::onRevoke, m_service, _1, _2, _3, _4, _5));
}

void
ManagerServiceNode::serveForever()
{
  addRoutes();
  m_node.serveForever();
}

} // nacapp