#include <ndn-cxx/util/string-helper.hpp>

#include "../src/common.hpp"
#include "../src/keychain.hpp"
#include "../src/manager.hpp"

#include "test.common.hpp"

namespace nacapp {
namespace tests {

const std::string PREFIX = "/prefix";
const std::uint32_t KEY_SIZE = 2048;

class ManagerTestFixture
{
public:
  ManagerTestFixture()
    : prefix(PREFIX)
    , entity(PREFIX + "/Alice")
    , dataType("health")
    , m_keychain(createMemoryKeyChain())
    , manager(PREFIX, m_keychain, KEY_SIZE)
  {
    ndn::RsaKeyParams keyParams(KEY_SIZE, ndn::KeyIdType::SHA256);
    Identity id = m_keychain->createIdentity(prefix, keyParams);
    m_keychain->setDefaultIdentity(id);
    m_ident = m_keychain->createIdentity(entity, keyParams);
    auto key = m_ident.getDefaultKey();
    m_cert = key.getDefaultCertificate();
    m_keychain->deleteIdentity(m_ident);
  }

public:
  const Identity&
  getIdentity()
  {
    return m_ident;
  }
  const Certificate&
  getCert()
  {
    return m_cert;
  }

public:
  const Name prefix;
  const Name entity;
  const Name dataType;
  shared_ptr<KeyChain> m_keychain;
  Manager manager;

private:
  Identity m_ident;
  Certificate m_cert;
};

TEST_CASE_METHOD(ManagerTestFixture, "Manager - addIdentity")
{
  Data certData(getCert());
  manager.addIdentity(entity, certData);
  shared_ptr<Certificate> got = manager.getIdentity(entity);
  REQUIRE(bool(got));
  REQUIRE(got->getName() == getCert().getName());
}

TEST_CASE_METHOD(ManagerTestFixture, "Manager - removeIdentity")
{
  Data certData(getCert());
  manager.addIdentity(entity, certData);
  shared_ptr<Certificate> got = manager.getIdentity(entity);
  REQUIRE(bool(got));
  manager.removeIdentity(entity);
  got = manager.getIdentity(entity);
  REQUIRE(!bool(got));
}

TEST_CASE_METHOD(ManagerTestFixture, "Manager - grantAccess")
{
  manager.addIdentity(entity, getCert());
  CHECK_NOTHROW(manager.grantAccess(entity, dataType));
  shared_ptr<GroupManager> group = manager.getGroup(dataType);
  REQUIRE(bool(group));
  ndn::gep::TimeStamp tp1(boost::posix_time::second_clock::local_time());
  std::list<Data> result = group->getGroupKey(tp1);
  REQUIRE(result.size() > 0);
}

TEST_CASE_METHOD(ManagerTestFixture, "Manager - revokeAccess")
{
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