#include <ndn-cxx/util/dummy-client-face.hpp>

#include "../../../shared/src/common-cxx.hpp"
#include "../../../shared/src/common-nac.hpp"
#include "../../../shared/src/common-ndn.hpp"
#include "../../../shared/src/keychain.hpp"
#include "../../../shared/test/catch.hpp"

#include "../../src/protocol/bootstrap.hpp"
#include "../../src/protocol/bt_session.hpp"
#include "../../src/protocol/bt_state_1.hpp"

namespace nacapp {
namespace protocol {
namespace test {
// class BootstrapTestFixture
// {
// public:
//   shared_ptr<Face> face = make_shared<ndn::util::DummyClientFace>();
//   shared_ptr<KeyChain> keychain = createMemoryKeyChain();
//   const Name device_prefix{"/local-home/bootstrap/device/SampleID123"};
//   const Name controller_prefix{"/local-home/bootstrap/owner"};
//   const string deviceId = "SampleID123";
//   const string pin = "UnSafe12345";
//   ndn::security::Identity identity = keychain->createIdentity(device_prefix);
// };

// TEST_CASE("Bootstrap Stage 1")
// {
//   BootstrapTestFixture fixture;
//   DeviceConfig cfg(fixture.device_prefix, fixture.controller_prefix, fixture.deviceId, fixture.pin);
//   BtSession session(fixture.identity);
//   WaitingForGwPubKey stage_1(fixture.face, fixture.keychain, session, cfg);
// }

} // namespace test
} // namespace protocol
} // namespace nacapp