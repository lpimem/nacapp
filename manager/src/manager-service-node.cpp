#include "manager-service-node.hpp"

namespace nacapp
{

void ManagerServiceNode::addRoutes()
{

  m_node.route("/READ", [&](const Interest &interest, const Name &args,
                            shared_ptr<Data> data, InterestShower show) {
    // args: <data-type>/E-Key/for/...
    ndn::name::Component keyType = args.get(1);
    const Name keyArgs = args.getSubName(3);
    if (keyType.toUri() == "E-Key")
      this->m_service.onGetEKey(interest, args, data, show);
    else if (keyType.toUri() == "D-Key")
      this->m_service.onGetDKey(interest, keyArgs, data, show);
  });

  //TODO: why this statement is not working
  // m_node.route("/IDENTITY/for", std::bind(&Service::onGetIdentityKey, service));

  m_node.route("/IDENTITY/for", [&](const Interest &interest, const Name &args,
                                    shared_ptr<Data> data, InterestShower show) {
    this->m_service.onGetIdentityKey(interest, args, data, show);
  });

  m_node.route("/MANAGEMENT/identity/add", [&](const Interest &interest, const Name &args,
                                               shared_ptr<Data> data, InterestShower show) {
    this->m_service.onAddIdentity(interest, args, data, show);
  });
  m_node.route("/MANAGEMENT/identity/remove", [&](const Interest &interest, const Name &args,
                                                  shared_ptr<Data> data, InterestShower show) {
    this->m_service.onRemoveIdentity(interest, args, data, show);
  });
  m_node.route("/MANAGEMENT/access/grant", [&](const Interest &interest, const Name &args,
                                               shared_ptr<Data> data, InterestShower show) {
    this->m_service.onGrant(interest, args, data, show);
  });
  m_node.route("/MANAGEMENT/access/revoke", [&](const Interest &interest, const Name &args,
                                                shared_ptr<Data> data, InterestShower show) {
    this->m_service.onRevoke(interest, args, data, show);
  });
}

void ManagerServiceNode::serveForever()
{
  addRoutes();
  m_node.serveForever();
}

} //nacapp