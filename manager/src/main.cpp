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
  auto manager = std::make_shared<nacapp::Manager>(PREFIX);
  nacapp::Service service(PREFIX, manager);
  service.serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}