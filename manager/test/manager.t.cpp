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

  Name dataType("health");

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

  SECTION("grantAccess") {
    // Block dataBlock = cert.wireEncode();
    // Data certData(dataBlock);
    // Name certName(entity);
    // certName.append("KEY").append("ksk-123").append("ID-CERT").append("123");
    // LOG(INFO) << certName.toUri();
    // certData.setName(certName);

    fixture.m.addIdentity(entity, cert);
    CHECK_NOTHROW(fixture.m.grantAccess(entity, dataType));
    shared_ptr<GroupManager> group = fixture.m.getGroup(dataType);
    REQUIRE(bool(group));
    ndn::gep::TimeStamp tp1(boost::posix_time::second_clock::local_time());
    std::list<Data> result = group->getGroupKey(tp1);
    REQUIRE(result.size() > 0);
  }

  SECTION("revokeAccess") {}
}

} // tests
} // nacapp