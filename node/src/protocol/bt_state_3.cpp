
#include <chrono>
#include <sstream>
#include <thread>

#include "bt_state_3.hpp"
#include "../common.hpp"


namespace nacapp {
namespace protocol {

void
WaitingForGwAuth::next()
{
  const int interestLifeTimeMs = 5000;
  LOG(INFO) << "BT stage 3 (WaitingForGwAuth) starts... ";
  Name n = nameOwnerAttestation(getConfig()->getControllerPrefix(), getConfig()->getId());

  Interest inst(n);
  inst.setMustBeFresh(true);
  inst.setInterestLifetime(time::milliseconds(interestLifeTimeMs));

  ndn::DataCallback onData = [this](const Interest& interest, const Data& data) {
    if (!this->isInSession()) {
      LOG(WARNING) << "WaitingForGwAuth onData not in session.";
      return;
    }

    LOG(INFO) << "R2: data: [" << data.getName().toUri() << "]\r\n\tcontent: "
              << ndn::toHex(data.getContent().value(), data.getContent().value_size(), true);
    this->onReceiveR2(this->getSession(), this->getConfig(), interest, data);
  };

  NackCallback onNack = [this](const Interest& i, const ndn::lp::Nack& n) {
    LOG(ERROR) << "NACK: " << i.getName().toUri() << " : " << n.getReason();
    std::ostringstream os;
    os << n.getReason();
    std::string reason = os.str();
    this->getSession()->onFailed(reason);
  };

  const int retry = 5;
  expressInterest(m_face, inst, retry, onData, onNack);
}

void
WaitingForGwAuth::onReceiveR2(BtSession* session,
                              shared_ptr<DeviceConfig> cfg,
                              const Interest& interest,
                              const Data& data)
{
  LOG(INFO) << "received data for " << interest.toUri();
  if (impl::validateR2(session, data)) {
    session->onSuc(cfg);
    session->setDone(true);
    LOG(INFO) << "bootstrap done. ";
  }
  else {
    session->onFailed("R2 dismatch");
  }
}

} // namespace protocol
} // namespace nacapp
