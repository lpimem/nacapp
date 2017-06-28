#include "manager-service-node.hpp"

namespace nacapp {

void
ManagerServiceNode::addRoutes()
{
  m_node.route("/READ",
               [&](const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower show,
                   PutData put) {
                 // args: <data-type>/E-Key/for/...
                 ndn::name::Component keyType = args.get(1);
                 const Name keyArgs = args.getSubName(3);
                 if (keyType.toUri() == "E-Key")
                   this->m_service.onGetEKey(interest, args, data, show, put);
                 else if (keyType.toUri() == "D-Key")
                   this->m_service.onGetDKey(interest, keyArgs, data, show, put);
               });

  m_node.route("/IDENTITY/for",
               std::bind(&Service::onGetIdentityKey, &m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/identity/add",
               std::bind(&Service::onAddIdentity, &m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/identity/remove",
               std::bind(&Service::onRemoveIdentity, &m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/access/grant",
               std::bind(&Service::onGrant, &m_service, _1, _2, _3, _4, _5));

  m_node.route("/MANAGEMENT/access/revoke",
               std::bind(&Service::onRevoke, &m_service, _1, _2, _3, _4, _5));
}

void
ManagerServiceNode::serveForever()
{
  addRoutes();
  m_node.serveForever();
}

} // nacapp