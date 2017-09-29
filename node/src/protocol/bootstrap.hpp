//
// Created by Lei Pi on 2/23/17.
//

#ifndef HOMESEC_BOOTSTRAP_HPP
#define HOMESEC_BOOTSTRAP_HPP

#include "bt_session.hpp"
#include "bt_state.hpp"
#include "bt_state_1.hpp"


namespace nacapp {
namespace protocol {
class BtState;

class Bootstrap : public std::enable_shared_from_this<Bootstrap>
{
public:
  Bootstrap(shared_ptr<Face> face,
            shared_ptr<KeyChain> keychain,
            const ndn::Name& device_prefix,
            const ndn::Name& controller_wellknown,
            ndn::security::Identity identity)
    : m_face(face)
    , m_keychain(keychain)
    , m_device_prefix(device_prefix)
    , m_controller_prefix(controller_wellknown)
    , m_session(new BtSession(identity))
  {
  }

  void
  startBootstrap(const string& deviceId,
                 const string& secret,
                 const OnBootstrapSuccess& onSuc,
                 const OnBootstrapFailed& onFailed);

  bool
  isDone()
  {
    return m_session->isDone();
  }

private:
  shared_ptr<Face> m_face;
  shared_ptr<KeyChain> m_keychain;
  Name m_device_prefix;
  Name m_controller_prefix;
  BtSession* m_session;
};

} // namespace protocol
} // namespace nacapp


#endif //HOMESEC_BOOTSTRAP_HPP
