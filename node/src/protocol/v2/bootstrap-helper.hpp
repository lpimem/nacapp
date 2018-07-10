#ifndef NODE_BOOTSTRAP_V2_HPP
#define NODE_BOOTSTRAP_V2_HPP

#include <functional>
#include <memory>
#include <string>

#include "../../../../shared/src/common-ndn.hpp"
#include "../../node.hpp"

namespace nacapp {

class BootstrapHelper;

using OnStatusChange = std::function<void(std::shared_ptr<Node>)>;

/**
 * Helper class to initate and manage bootstrapping sessions. 
 * 
 * shared_ptr<Node> n = make_shared<Node>(...);  
 * BootstrapHelper bootstrap(n);
 * bootstrap.start("/local-home/owner", "dev123", "unsafe", 
 * [](std::shared_ptr<BootstrapHelper> bt){
 *    std::cout << bt.getDeviceCert().getName().toUri() << std::endl;
 *    std::cout << bt.getOwnerCert().getName().toUri() << std::endl;
 * }, 
 * [](std::shared_ptr<BootstrapHelper> bt){
 *    std::cerr << bt.getError() << std::endl;
 * })
 * 
 */
class BootstrapHelper
{
public:
  BootstrapHelper(std::shared_ptr<Node> n)
    : m_node(n)
  {
  }

public:
  /**
   * start the bootstrapping session as a device
   */
  void
  start(Name ownerName, std::string deviceId, std::string pin, OnStatusChange, OnStatusChange);

  /**
   * start serving bootstrap sessions as an owner
   */
  void
  startService(Name ownerName, std::string pin, OnStatusChange, OnStatusChange);

  /**
   * stop the current process.
   */
  void
  stop();

public:
  std::shared_ptr<Node>
  getNode()
  {
    return m_node;
  }

  std::shared_ptr<Certificate>
  getDeviceCert()
  {
    return m_deviceCert;
  }

  std::shared_ptr<Certificate>
  getOwnerCert()
  {
    return m_ownerCert;
  }

  std::string
  getError()
  {
    return m_error;
  }

public:
  std::string
  getDeviceId()
  {
    return m_deviceId;
  }

  std::string
  getDevicePin()
  {
    return m_pin;
  }

private:
  /**
   * TODO: broadcasting the first interest using all possible faces on NFD and 
   * add route entries upon authenticated response data.
   */
  bool
  beacon();

private:
  enum class BTStatus { STARTED, SERVING, TERMINATED, SUCCESS, FAIL };

private:
  std::shared_ptr<Certificate> m_deviceCert;
  std::shared_ptr<Certificate> m_ownerCert;

  OnStatusChange m_onSuccess;
  OnStatusChange m_onFailure;

  std::string m_error;
  BTStatus m_status;
  std::string m_deviceId;
  std::string m_pin;
  Name m_ownerName;
  std::shared_ptr<Node> m_node;
};

} // namespace nacapp

#endif