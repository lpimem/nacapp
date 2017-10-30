#ifndef BT_STATE_3_HPP
#define BT_STATE_3_HPP

#include "bt_state.hpp"
#include "details/bootstrap_impl.hpp"


namespace nacapp {
namespace protocol {
class WaitingForGwAuth : public BtState
{
public:
  WaitingForGwAuth(shared_ptr<Face> face,
                   shared_ptr<KeyChain> keychain,
                   shared_ptr<BtSession> session,
                   shared_ptr<DeviceConfig> cfg)
    : BtState(face, keychain, session, cfg, BtStage::FetchingGwAuth)
  {
  }

  void
  next();

protected:
  static void
  onReceiveR2(shared_ptr<BtSession> session,
              shared_ptr<DeviceConfig> cfg,
              const Interest&,
              const Data&);
};

} // namespace protocol
} // namespace nacapp

#endif /* BT_STATE_3_HPP */
