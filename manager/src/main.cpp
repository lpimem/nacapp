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
  auto keychain = nacapp::createMemoryKeyChain();
  nacapp::ManagerServiceNode node(PREFIX, f, keychain);
  node.serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}