#include "../../node/src/node.hpp"
#include "common.hpp"
#include "manager.hpp"
#include "service.hpp"

INITIALIZE_EASYLOGGINGPP

/**
 * main function of manager application
 */
int main() {
  LOG(INFO) << ">>> START >>>";
  const std::string PREFIX = "/local-home";
  auto f = std::make_shared<ndn::Face>("127.0.0.1");
  shared_ptr<nacapp::Node> node = makeNode(PREFIX);
  node->serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}

shared_ptr<nacapp::Node> makeNode(const std::string prefix,
                                  shared_ptr<ndn::Face> f) {
  auto node = make_shared<nacapp::Node>(prefix, f);
  auto service = makeService(prefix, f);
  addRoutes(ndoe, service);
  return node;
}

shared_ptr<nacapp::Service> makeService(const std::string prefix,
                                        shared_ptr<ndn::Face> f) {}

void addRoutes(shared_ptr<nacapp::Node> node,
               shared_ptr<nacapp::Service> service) {}