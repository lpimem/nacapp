#ifndef BOOTSTRAP_IMPL_HPP
#define BOOTSTRAP_IMPL_HPP

#include "../bt_common.hpp"
#include "../bt_state.hpp"


namespace nacapp {
namespace protocol {

class Bootstrap;

class Step3Payload
{
public:
  Step3Payload(std::string p1, std::string p2)
    : R2(p1)
    , certEncoded(p2)
  {
  }

public:
  std::string R2;
  std::string certEncoded;
};

class BtHelper
{
public:
  static void
  defaultOnNackReceived(const Interest& i, const ndn::lp::Nack& n)
  {
    LOG(ERROR) << "NACK: " << i.getName().toUri() << " : " << n.getReason();
    throw "nack " + i.getName().toUri();
  }

  static void
  defaultOnRegisterPrefixFailure(const Name& prefix, const std::string& reason)
  {
    LOG(ERROR) << "Register Prefix: " << prefix.toUri() << " : " << reason;
  }

  static void
  defaultOnTimeout(const Interest& interest)
  {
    LOG(ERROR) << "Timeout: " << interest.toUri();
    throw "timeout: " + interest.toUri();
  }
};

namespace impl {
shared_ptr<Buffer>
generateDevicePublicKey(DeviceConfig& cfg);

ndn::ConstBufferPtr
generateSecret(const std::string& pin);

bool
authenticateInterest(const ndn::Interest& interest, ndn::ConstBufferPtr& key);

void
parseGwPubKey(shared_ptr<BtSession> session, const ndn::Data& data, shared_ptr<DeviceConfig> config);

void
publishDeviceKey(shared_ptr<Face> face,
                 shared_ptr<ndn::KeyChain> kc,
                 shared_ptr<BtSession> session,
                 shared_ptr<DeviceConfig> cfg,
                 const Name& name);

Step3Payload
parseStep3Payload(shared_ptr<BtSession> session, const ndn::Data& data);

void
validateR2(shared_ptr<BtSession> session, const Step3Payload& payload);

void
parseDeviceCertificate(shared_ptr<BtSession> session, const Step3Payload& payload);

} // namespace impl
} // namespace protocol
} // namespace nacapp


#endif /* BOOTSTRAP_IMPL_HPP */
