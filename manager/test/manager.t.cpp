#include <ndn-cxx/util/string-helper.hpp>

#include "../../shared/test/catch.hpp"

#include "../src/common.hpp"
#include "../src/keychain.hpp"
#include "../src/manager.hpp"

namespace nacapp
{
namespace tests
{

const std::string PREFIX = "/prefix";
const std::uint32_t KEY_SIZE = 2048;

class ManagerTestFixture
{

public:
  ManagerTestFixture()
      : prefix(PREFIX), entity(PREFIX + "/Alice"), dataType("health"),
        manager(PREFIX, KEY_SIZE)
  {
    ndn::RsaKeyParams keyParams(KEY_SIZE, ndn::KeyIdType::SHA256);
    // ndn::RsaKeyParams keyParams;

    // LOG(DEBUG) << keyParams.getKeyType();
    // LOG(DEBUG) << keyParams.getKeySize();
    Identity id = AppKeyChain.createIdentity(prefix, keyParams);
    // const ndn::KeyParams keyParams = getDefaultKeyParams();
    // Identity id = AppKeyChain.createIdentity(prefix, keyParams);
    AppKeyChain.setDefaultIdentity(id);
    m_ident = AppKeyChain.createIdentity(entity, keyParams);
    auto key = m_ident.getDefaultKey();
    m_cert = key.getDefaultCertificate();
    AppKeyChain.deleteIdentity(m_ident);
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
  // string certHex = ndn::toHex(*getCert().getContent().getBuffer());
  // LOG(DEBUG) << certHex;
  // LOG(DEBUG) << "key size: " << getCert().getPublicKey().size() * 8;
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