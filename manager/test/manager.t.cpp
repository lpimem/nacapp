#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/common.hpp"
#include "../src/manager.hpp"

namespace nacapp {
namespace tests {

const std::string PREFIX = "/prefix";

Manager createManager() {
  Manager m(PREFIX);
  return m;
}

TEST_CASE("Manager") {

  SECTION("addIdentity") {}

  SECTION("removeIdentity") {}

  SECTION("grantAccess") {}

  SECTION("revokeAccess") {}
}

} // tests
} // nacapp