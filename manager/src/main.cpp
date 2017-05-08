#include "../../node/src/node.hpp"

#include "service.hpp"

INITIALIZE_EASYLOGGINGPP

std::shared_ptr<nacapp::Service> makeService(const std::string prefix,
                                             std::shared_ptr<ndn::Face> f) {
  return NULL;
}

void addRoutes(std::shared_ptr<nacapp::Node> node,
               std::shared_ptr<nacapp::Service> service) {}

std::shared_ptr<nacapp::Node> makeNode(const std::string prefix,
                                       std::shared_ptr<ndn::Face> f) {
  auto node = std::make_shared<nacapp::Node>(prefix, f);
  auto service = makeService(prefix, f);
  addRoutes(node, service);
  return node;
}

/**
* main function of manager application
*/
int main() {
  LOG(INFO) << ">>> START >>>";
  const std::string PREFIX = "/local-home";
  auto f = std::make_shared<ndn::Face>("127.0.0.1");
  std::shared_ptr<nacapp::Node> node = makeNode(PREFIX, f);
  node->serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}