
#include "manager-service-node.hpp"

#include <ndn-cxx/encoding/tlv.hpp>

namespace nacapp {

namespace manager {
// returns prefix before a token in a name
Name
extractPrefix(const Name& name, const ndn::name::Component& token)
{
  size_t pos = 0;
  for (; pos < name.size(); pos++) {
    ndn::name::Component c = name.get(pos);
    if (c == token) {
      break;
    }
  }
  return name.getPrefix(pos);
}

std::vector<Name>
parseReadArgs(const Name& args)
{
  size_t pos = 0;
  for (; pos < args.size(); pos++) {
    ndn::name::Component c = args.get(pos);
    if (c == NAME_COMPONENT_E_KEY || c == NAME_COMPONENT_D_KEY) {
      break;
    }
  }
  Name location = args.getSubName(0, pos);
  Name keyType = args.getSubName(pos, 1);
  Name keyArgs = args.getSubName(pos + 1);
  return std::vector<Name>{location, keyType, keyArgs};
}
}

void
ManagerServiceNode::addRoutes()
{
  m_node.route("/READ",
               [&](const Interest& interest,
                   const Name& args,
                   shared_ptr<Data> data,
                   InterestShower show,
                   PutData put) {

                 std::vector<Name> parts = manager::parseReadArgs(args);
                 Name location = parts[0];
                 Name keyType = parts[1];
                 Name keyArgs = parts[2];

                 LOG(INFO) << "[DEBUG] keyType: " << keyType.toUri();

                 if (keyType.toUri() == "/E-KEY")
                   return this->m_service->onGetEKey(interest, location, keyArgs, data, show, put);
                 else if (keyType.toUri() == "/D-KEY")
                   return this->m_service->onGetDKey(interest, location, keyArgs, data, show, put);
                 else {
                   string msg{"invalid pattern for path /READ"};
                   LOG(ERROR) << msg;
                   data->setContentType(ndn::tlv::ContentType_Nack);
                   data->setContent(reinterpret_cast<const uint8_t*>(msg.c_str()), msg.length());
                   return false;
                 }
               });

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