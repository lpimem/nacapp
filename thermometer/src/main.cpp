#include "common.hpp"
#include "thermometer.hpp"

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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


int
main()
{
  signal(SIGSEGV, crash_handler);

  const std::string PREFIX = "/local-home/NAC";
  const std::string LOCATION = "/location/bedroom";
  const std::string DTYPE = LOCATION + "/temperature";
  const std::string DB = "./test.db";

  auto f = std::make_shared<ndn::Face>("127.0.0.1");

  auto keychain = nacapp::createMemoryKeyChain();
  nacapp::thermometer::Thermometer t{PREFIX, LOCATION, DTYPE, f, DB, keychain};
  t.run();
  return 0;
}