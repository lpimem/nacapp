#include "bootstrap.hpp"

#include "../../../../shared/src/hmac.hpp"
#include "../../../../shared/src/ndn-util.hpp"
#include "../../../../shared/src/string_helpers.hpp"

#include "bootstrap-impl.hpp"

namespace nacapp {

/** 
 *  node -..-> owner 
 *    r1 + devid + hmac
 *  owner -> node 
 *    r1 + owner cert + r2
 *  node -> owner 
 *    r2 +  
 */
void
BootstrapHelper::start(Name ownerName,
                       std::string deviceId,
                       std::string pin,
                       OnStatusChange onSuccess,
                       OnStatusChange onFail)
{
  m_deviceId = deviceId;
  m_pin = pin;
  m_ownerName = ownerName;
  m_onSuccess = onSuccess;
  m_onFailure = onFail;
  m_status = BootstrapHelper::BTStatus::STARTED;
  bootstrap::startBootstrap(ownerName, deviceId, shared_from_this(), m_onSuccess, m_onFailure);
}

} // namespace nacapp