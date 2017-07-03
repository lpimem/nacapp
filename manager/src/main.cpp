#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../node/src/node.hpp"

#include "manager-service-node.hpp"

INITIALIZE_EASYLOGGINGPP

void
crash_handler(int sig)
{
  void* array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}


/**
* main function of manager application
*/
int
main()
{
  signal(SIGSEGV, crash_handler);

  LOG(INFO) << ">>> START >>>";
  const std::string PREFIX = "/local-home/NAC";
  auto f = std::make_shared<ndn::Face>("127.0.0.1");
  auto keychain = nacapp::createMemoryKeyChain();
  nacapp::ManagerServiceNode node(PREFIX, f, keychain);

  // debug configurations
  // std::shared_ptr<nacapp::Manager> manager = node.getService()->getManager();
  // manager->grantAccess("/local-home/dev/tempreader", "/location/bedroom/temperature");
  // manager->grantAccess("/Alice", "/location/bedroom/temperature");
  // end of debug configurations

  node.serveForever();
  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}