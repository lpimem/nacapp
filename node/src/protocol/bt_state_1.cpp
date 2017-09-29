#include "bt_state_1.hpp"
#include "bt_state_2.hpp"
#include "../common.hpp"
#include "details/bootstrap_impl.hpp"

#include <sstream>


namespace nacapp {
namespace protocol {

void
WaitingForGwPubKey::next()
{
  const Name keyname = nameOwnerPublicKey(m_config->getControllerPrefix(), m_config->getId());

  Interest interest(keyname);
  interest.setMustBeFresh(true);
  interest.setInterestLifetime(time::milliseconds(1000));

  auto onNack = [&](const Interest& i, const ndn::lp::Nack& n) {
    LOG(ERROR) << "NACK: " << i.getName().toUri() << " : " << n.getReason();
    std::ostringstream os;
    os << n.getReason();
    std::string reason = os.str();
    this->getSession()->onFailed(reason);
  };

  auto onData = [this](const Interest& interest, const Data& data) {
    if (!this->isInSession()) {
      LOG(WARNING) << "WaitingForGwPubKey onData not in session.";
      return;
    }
    LOG(INFO) << "received data from " << data.getName().toUri();
    this->onReceivedPubKey(interest, data);
  };

  const int retry = 5;
  expressInterest(m_face, interest, retry, onData, onNack);
}

void
WaitingForGwPubKey::onReceivedPubKey(const ndn::Interest& inst, const ndn::Data& data)
{
  auto session = getSession();
  auto config = getConfig();
  impl::parseGwPubKey(session, data, config);
  shared_ptr<BtState> state = std::dynamic_pointer_cast<BtState>(
    make_shared<PublishingDevicePubKey>(m_face, m_keychain, session, config));
  state->setR1(session->getCurrentState()->getR1());
  session->setState(state);
  state->next();
}

} // namespace protocol
} // namespace nacapp
