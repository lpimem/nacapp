#ifndef BT_STATE_1_HPP
#define BT_STATE_1_HPP

#include "bt_session.hpp"
#include "bt_state.hpp"


namespace nacapp {
namespace protocol {

class WaitingForGwPubKey : public BtState
{
public:
  WaitingForGwPubKey(shared_ptr<Face> face,
                     shared_ptr<KeyChain> keychain,
                     shared_ptr<BtSession> session,
                     shared_ptr<DeviceConfig> cfg)
    : BtState(face, keychain, session, cfg, BtStage::FetchingGwPubkey)
  {
  }

  void
  next();

protected:
  void
  onReceivedPubKey(const Interest&, const Data&);
};
} // namespace protocol
} // namespace nacapp

#endif /* BT_STATE_1_HPP */
