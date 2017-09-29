#ifndef BT_STATE_2_HPP
#define BT_STATE_2_HPP

#include "bt_state.hpp"


namespace nacapp {
namespace protocol {
class PublishingDevicePubKey : public BtState
{
public:
  PublishingDevicePubKey(shared_ptr<Face> face,
                         shared_ptr<KeyChain> keychain,
                         BtSession* session,
                         shared_ptr<DeviceConfig> cfg)
    : BtState(face, keychain, session, cfg, BtStage::PublishingSelfPubkey)
    , m_registered_prefix_id(nullptr)
  {
  }

  // ~PublishingDevicePubKey();

  void
  next();

protected:
  void
  onRequestDeviceKey(const InterestFilter& prefix, const Interest& interest);

private:
  void
  onInterest(const InterestFilter& filter, const Interest& interest);

private:
  RegisteredPrefixId* m_registered_prefix_id;
};

} // namespace protocol
} // namespace nacapp

#endif /* BT_STATE_2_HPP */
