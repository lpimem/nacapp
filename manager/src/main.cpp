#include "../../node/src/node.hpp"

#include "service.hpp"

INITIALIZE_EASYLOGGINGPP

namespace nacapp
{

void addRoutes(std::shared_ptr<nacapp::Node> node,
               std::shared_ptr<nacapp::Service> service)
{
  node->route("/READ", [&](const Interest &interest, const Name &args,
                           shared_ptr<Data> data, InterestShower show) {
    // args: <data-type>/E-Key/for/...
    ndn::name::Component keyType = args.get(1);
    const Name keyArgs = args.getSubName(3);
    if (keyType.toUri() == "E-Key")
      service->onGetEKey(interest, args, data, show);
    else if (keyType.toUri() == "D-Key")
      service->onGetDKey(interest, keyArgs, data, show);
  });

  //TODO: why this statement is not working
  // node->route("/IDENTITY/for", std::bind(&Service::onGetIdentityKey, service));

  node->route("/IDENTITY/for", [&](const Interest &interest, const Name &args,
                                   shared_ptr<Data> data, InterestShower show) {
    service->onGetIdentityKey(interest, args, data, show);
  });

  node->route("/MANAGEMENT/identity/add", [&](const Interest &interest, const Name &args,
                                              shared_ptr<Data> data, InterestShower show) {
    service->onAddIdentity(interest, args, data, show);
  });
  node->route("/MANAGEMENT/identity/remove", [&](const Interest &interest, const Name &args,
                                                 shared_ptr<Data> data, InterestShower show) {
    service->onRemoveIdentity(interest, args, data, show);
  });
  node->route("/MANAGEMENT/access/grant", [&](const Interest &interest, const Name &args,
                                              shared_ptr<Data> data, InterestShower show) {
    service->onGrant(interest, args, data, show);
  });
  node->route("/MANAGEMENT/access/revoke", [&](const Interest &interest, const Name &args,
                                               shared_ptr<Data> data, InterestShower show) {
    service->onRevoke(interest, args, data, show);
  });
}

std::shared_ptr<nacapp::Node> makeNode(const std::string prefix,
                                       std::shared_ptr<ndn::Face> f)
{
  auto node = std::make_shared<nacapp::Node>(prefix, f);
  auto service = std::make_shared<nacapp::Service>(prefix);
  addRoutes(node, service);
  return node;
}
} // nacapp

/**
* main function of manager application
*/
int main()
{
  LOG(INFO) << ">>> START >>>";
  const std::string PREFIX = "/local-home";
  auto f = std::make_shared<ndn::Face>("127.0.0.1");
  std::shared_ptr<nacapp::Node> node = nacapp::makeNode(PREFIX, f);
  node->serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}