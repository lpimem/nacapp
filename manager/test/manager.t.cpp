#include "../../shared/test/catch.hpp"

#include "../src/common.hpp"
#include "../src/keychain.hpp"
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