#include "bt_state.hpp"


namespace nacapp {
namespace protocol {

ndn::security::Identity
BtSession::getDevId() const
{
  return m_deviceIdentity;
}

bool
BtSession::isDone()
{
  return m_done;
}

void
BtSession::setDone(bool done)
{
  m_done = done;
}

} // namespace protocol
} // namespace nacapp
