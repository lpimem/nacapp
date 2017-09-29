#ifndef HOMESEC_BT_SESSION_HPP
#define HOMESEC_BT_SESSION_HPP

#include "bt_common.hpp"
#include "bt_state.hpp"


namespace nacapp {
namespace protocol {

class BtState;

using OnBootstrapSuccess = std::function<void(shared_ptr<DeviceConfig>)>;
using OnBootstrapFailed = std::function<void(std::string message)>;

class BtSession
{
public:
  BtSession(ndn::security::Identity id)
    : m_onSucHandler(nullptr)
    , m_onFailedHandler(nullptr)
    , m_deviceIdentity(id)
    , m_currentState(nullptr)
    , m_done(false)
  {
  }

public:
  shared_ptr<BtState>
  getCurrentState() const
  {
    return m_currentState;
  }

  void
  setState(shared_ptr<BtState> s)
  {
    m_currentState = s;
  }

  ndn::security::Identity
  getDevId() const;

  bool
  isDone();

  void
  setDone(bool done);

public:
  void
  setOnSuccessHandler(const OnBootstrapSuccess& handler)
  {
    m_onSucHandler = handler;
  }

  void
  setOnFailHandler(const OnBootstrapFailed& handler)
  {
    m_onFailedHandler = handler;
  }

  void
  onSuc(shared_ptr<DeviceConfig> config)
  {
    if (m_onSucHandler != nullptr) {
      (m_onSucHandler)(config);
    }
  }

  void
  onFailed(std::string message)
  {
    if (m_onFailedHandler != nullptr) {
      (m_onFailedHandler)(message);
    }
  }

private:
  OnBootstrapSuccess m_onSucHandler;
  OnBootstrapFailed m_onFailedHandler;
  const ndn::security::Identity m_deviceIdentity;
  shared_ptr<BtState> m_currentState;
  bool m_done;
};

} // namespace protocol
} // namespace nacapp


#endif /* HOMESEC_BT_SESSION_HPP */