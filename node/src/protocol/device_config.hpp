#ifndef DEVICE_CONFIG_HPP
#define DEVICE_CONFIG_HPP

#include <string>

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/util/string-helper.hpp>

#include "../common.hpp"


namespace nacapp {
namespace protocol {

/**
 * Dynamic configurations of a device, filled during bootstrapping 
 * process.
 */
class DeviceConfig
{
public:
  DeviceConfig(const ndn::Name& device_prefix,
               const ndn::Name& controller_wellknown,
               std::string devId,
               std::string pin)
    : m_device_prefix(device_prefix)
    , m_controller_prefix(controller_wellknown)
    , m_id(devId)
    , m_pin(pin)
  {
  }

  const Name&
  getDevicePrefix()
  {
    return m_device_prefix;
  }

  const Name&
  getControllerPrefix()
  {
    return m_controller_prefix;
  }

  /**
   * TODO: this is actully the owner's key
   */
  Buffer
  getGwPubKey() const;

  /**
   * TODO: this is actully the owner's key
   */
  void
  setGwPubKey(Buffer gwPubKey);

  Buffer
  getDevicePubKey() const;

  void
  setDevicePubKey(Buffer devicePubKey);

  const std::string
  getId() const;

  const std::string
  getPin() const;

private:
  Name m_device_prefix;
  Name m_controller_prefix;
  std::string m_id;
  std::string m_pin;
  Buffer m_gwPubKey;
  Buffer m_devicePubKey;
};

} // namespace protocol
} // namespace nacapp


#endif /* DEVICE_CONFIG_HPP */
