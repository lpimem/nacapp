#include "../../shared/test/catch.hpp"

#include "../src/common.hpp"
#include "../src/keychain.hpp"
#include "../src/manager.hpp"

namespace nacapp {
namespace tests {

const std::string PREFIX = "/prefix";

class ManagerFixture {
public:
  ManagerFixture(string prefix) : m(prefix) {}

public:
  Manager m;
};

TEST_CASE("Manager") {
  ManagerFixture fixture(PREFIX);
  Name entity(PREFIX);
  entity.append("Alice");
  Identity ident = AppKeyChain.createIdentity(entity);
  auto key = ident.getDefaultKey();
  Certificate cert = key.getDefaultCertificate();

  SECTION("addIdentity") {
    Data certData(cert);
    fixture.m.addIdentity(entity, certData);
    shared_ptr<Certificate> got = fixture.m.getIdentity(entity);
    REQUIRE(bool(got));
    REQUIRE(got->getName() == cert.getName());
  }

  SECTION("removeIdentity") {
    Data certData(cert);
    fixture.m.addIdentity(entity, certData);
    shared_ptr<Certificate> got = fixture.m.getIdentity(entity);
    REQUIRE(bool(got));
    fixture.m.removeIdentity(entity);
    got = fixture.m.getIdentity(entity);
    REQUIRE(!bool(got));
  }

  SECTION("grantAccess") {}

  SECTION("revokeAccess") {}
}

} // tests
} // nacapp