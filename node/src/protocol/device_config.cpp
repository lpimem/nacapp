#include "device_config.hpp"


namespace nacapp {
namespace protocol {

ndn::Buffer
DeviceConfig::getGwPubKey() const
{
  return m_gwPubKey;
}

void
DeviceConfig::setGwPubKey(Buffer gwPubKey)
{
  DeviceConfig::m_gwPubKey = gwPubKey;
}

Buffer
DeviceConfig::getDevicePubKey() const
{
  return m_devicePubKey;
}

void
DeviceConfig::setDevicePubKey(ndn::Buffer devicePubKey)
{
  DeviceConfig::m_devicePubKey = devicePubKey;
}

const std::string
DeviceConfig::getId() const
{
  return m_id;
}

const std::string
DeviceConfig::getPin() const
{
  return m_pin;
}

} // namespace protocol
} // namespace nacapp
