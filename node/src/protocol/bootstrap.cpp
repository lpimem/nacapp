//
// Created by Lei Pi on 2/23/17.
//
#include "bootstrap.hpp"


namespace nacapp {
namespace protocol {

void
Bootstrap::startBootstrap(const string& deviceId,
                          const string& secret,
                          const OnBootstrapSuccess& onSuc,
                          const OnBootstrapFailed& onFailed)
{
  LOG(INFO) << "start bootstrapping...";
  // register handlers
  m_session->setOnSuccessHandler(onSuc);
  m_session->setOnFailHandler(onFailed);
  // init configuration
  auto config = make_shared<DeviceConfig>(m_device_prefix, m_controller_prefix, deviceId, secret);
  ndn::security::Key deviceKey = m_session->getDevId().getDefaultKey();
  const Buffer pubkeyBuf = deviceKey.getPublicKey();
  config->setDevicePubKey(pubkeyBuf);
  // start bt session
  shared_ptr<BtState> initState = std::dynamic_pointer_cast<BtState>(
    make_shared<WaitingForGwPubKey>(m_face, m_keychain, m_session, config));
  m_session->setState(initState);
  initState->next();
}

} // namespace protocol
} // namespace nacapp
