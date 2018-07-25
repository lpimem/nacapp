#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.hpp"
#include "../../node/src/node.hpp"


INITIALIZE_EASYLOGGINGPP


/**
 * crash_handler is called when the program crashed 
 * unexceptedly. It will dump the call backtrack to
 * stderr.
 */
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


class App
{
private:
  nacapp::Node m_node;
  std::shared_ptr<KeyChain> m_keyChain;
};


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

  LOG(INFO) << "<<< EXIT <<<";
  return 0;
}