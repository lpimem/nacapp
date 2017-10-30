#include "bt_state.hpp"


namespace nacapp {
namespace protocol {

shared_ptr<DeviceConfig>
BtState::getConfig() const
{
  return m_config;
}

void
BtState::setConfig(shared_ptr<DeviceConfig> config)
{
  m_config = config;
}

BtStage
BtState::getStage() const
{
  return m_stage;
}

void
BtState::setStage(BtStage stage)
{
  m_stage = stage;
}

shared_ptr<BtSession>
BtState::getSession()
{
  return m_session;
}

std::string
BtState::getR1() const
{
  return r1;
}

void
BtState::setR1(const std::string& r1)
{
  BtState::r1 = r1;
}

std::string
BtState::getR2() const
{
  return r2;
}

void
BtState::setR2(const std::string& r2)
{
  BtState::r2 = r2;
}

bool
BtState::isInSession()
{
  return m_session->getCurrentState().get() == this;
}

} // namespace protocol
} // namespace nacapp
