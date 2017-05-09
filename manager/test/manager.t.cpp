#include <ndn-cxx/util/string-helper.hpp>

#include "../../shared/test/catch.hpp"

#include "../src/common.hpp"
#include "../src/keychain.hpp"
#include "../src/manager.hpp"

namespace nacapp {
namespace tests {

const std::string PREFIX = "/prefix";

class ManagerTestFixture {

public:
  ManagerTestFixture()
      : prefix(PREFIX), entity(PREFIX + "/Alice"), dataType("health"),
        manager(PREFIX) {
    Identity id = AppKeyChain.createIdentity(prefix, getDefaultKeyParams());
    AppKeyChain.setDefaultIdentity(id);
    m_ident = AppKeyChain.createIdentity(entity, getDefaultKeyParams());
    auto key = m_ident.getDefaultKey();
    m_cert = key.getDefaultCertificate();
  }

public:
  const Identity &getIdentity() { return m_ident; }
  const Certificate &getCert() { return m_cert; }

public:
  const Name prefix;
  const Name entity;
  const Name dataType;
  Manager manager;

private:
  Identity m_ident;
  Certificate m_cert;
};

TEST_CASE_METHOD(ManagerTestFixture, "Manager - addIdentity") {

  Data certData(getCert());
  manager.addIdentity(entity, certData);
  bool idexist = AppKeyChain.getPib().getIdentities().find(entity) !=
                 AppKeyChain.getPib().getIdentities().end();
  REQUIRE(idexist);
  shared_ptr<Certificate> got = manager.getIdentity(entity);
  REQUIRE(bool(got));
  REQUIRE(got->getName() == getCert().getName());
}

TEST_CASE_METHOD(ManagerTestFixture, "Manager - removeIdentity") {
  Data certData(getCert());
  manager.addIdentity(entity, certData);
  shared_ptr<Certificate> got = manager.getIdentity(entity);
  REQUIRE(bool(got));
  manager.removeIdentity(entity);
  got = manager.getIdentity(entity);
  REQUIRE(!bool(got));
}

TEST_CASE_METHOD(ManagerTestFixture, "Manager - grantAccess") {
  string certHex = ndn::toHex(*getCert().getContent().getBuffer());

  LOG(DEBUG) << certHex;

  manager.addIdentity(entity, getCert());
  CHECK_NOTHROW(manager.grantAccess(entity, dataType));
  shared_ptr<GroupManager> group = manager.getGroup(dataType);
  REQUIRE(bool(group));

  ndn::gep::TimeStamp tp1(boost::posix_time::second_clock::local_time());
  LOG(DEBUG) << "TEST 111111";
  std::list<Data> result = group->getGroupKey(tp1);
  LOG(DEBUG) << "TEST 222222";

  REQUIRE(result.size() > 0);
}

TEST_CASE_METHOD(ManagerTestFixture, "Manager - revokeAccess") {
  manager.addIdentity(entity, getCert());
  CHECK_NOTHROW(manager.grantAccess(entity, dataType));
  shared_ptr<GroupManager> group = manager.getGroup(dataType);
  REQUIRE(bool(group));
  CHECK_NOTHROW(manager.revokeAccess(entity, dataType));
  group = manager.getGroup(dataType);
  REQUIRE(!bool(group));
}

} // tests
} // nacapp