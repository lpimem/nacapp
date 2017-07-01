#include "../../node/src/node.hpp"

#include "manager-service-node.hpp"

INITIALIZE_EASYLOGGINGPP

/**
* main function of manager application
*/
int
main()
{
  LOG(INFO) << ">>> START >>>";
  const std::string PREFIX = "/local-home/manager";
  auto f = std::make_shared<ndn::Face>("127.0.0.1");
  auto keychain = std::make_shared<ndn::security::v2::KeyChain>("pib-memory:", "tpm-memory:", true);
  nacapp::ManagerServiceNode node(PREFIX, f, keychain);
  node.serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}