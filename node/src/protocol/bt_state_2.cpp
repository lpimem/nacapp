#include "bt_state_2.hpp"
#include "bt_state_3.hpp"
#include "details/bootstrap_impl.hpp"


namespace nacapp {
namespace protocol {

void
unregisterPrefix(shared_ptr<Face> face, ndn::RegisteredPrefixId* pid)
{
  LOG(INFO) << "BT Stage 2: unregistering prefix " << pid;
  face->unsetInterestFilter(pid);
}

void
PublishingDevicePubKey::next()
{
  LOG(INFO) << "BT stage 2(PublishingDevicePubKey) starts...";
  Name name = nameDevicePublicKey(getConfig()->getDevicePrefix());
  LOG(INFO) << "set interest filter: " << name;
  const RegisteredPrefixId* prefixId =
    m_face->setInterestFilter(name,
                              bind(&PublishingDevicePubKey::onInterest, this, _1, _2),
                              RegisterPrefixSuccessCallback(),
                              BtHelper::defaultOnRegisterPrefixFailure);
  m_registered_prefix_id = const_cast<RegisteredPrefixId*>(prefixId);
}

void
PublishingDevicePubKey::onInterest(const InterestFilter& prefix, const Interest& interest)
{
  LOG(INFO) << "PublishingDevicePubKey callback";
  if (!this->isInSession()) {
    LOG(WARNING) << "PublishingDevicePubKey onInterest not in session.";
    return;
  }
  if (this->isProcessing()) {
    LOG(INFO) << "alreading processing, interest dropped";
    return;
  }
  this->flagProcessing();
  // unregisterPrefix(m_face, m_registered_prefix_id);
  this->onRequestDeviceKey(prefix, interest);
  this->flagProcessed();
}

void
PublishingDevicePubKey::onRequestDeviceKey(const ndn::InterestFilter& prefix, const Interest& interest)
{
  auto session = getSession();
  auto cfg = getConfig();

  impl::publishDeviceKey(m_face, m_keychain, session, cfg, prefix.getPrefix());
  // make sure data is sent.
  m_face->processEvents(time::milliseconds(1000));
  shared_ptr<BtState> state = std::dynamic_pointer_cast<BtState>(
    make_shared<WaitingForGwAuth>(m_face, m_keychain, session, cfg));
  state->setR2(session->getCurrentState()->getR2());
  session->setState(state);
  state->next();
}

} // namespace protocol
} // namespace nacapp
