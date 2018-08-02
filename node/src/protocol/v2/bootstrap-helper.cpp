#include "bootstrap-helper.hpp"

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
                       std::shared_ptr<ndn::KeyChain> keychain,
                       OnStatusChange onSuccess,
                       OnStatusChange onFail)
{
  m_deviceId = deviceId;
  m_pin = pin;
  m_ownerName = ownerName;
  m_onSuccess = [&](std::shared_ptr<Node> n) {
    n->setTrustAnchor(*m_ownerCert);
    onSuccess(n);
  };
  m_onFailure = onFail;
  m_status = BootstrapHelper::BTStatus::STARTED;

  bootstrap::OnOwnerCert cbk = [&](std::shared_ptr<Certificate> cert) {
    LOG(INFO) << "Extracted owner cert: " << cert->getName().toUri();
    m_ownerCert = cert;
  };
  bootstrap::startBootstrap(ownerName,
                            deviceId,
                            pin,
                            keychain,
                            m_node,
                            cbk,
                            [&](const ndn::Name& dname, shared_ptr<Certificate> cert) {
                              m_deviceCert = cert;
                              auto id = keychain->createIdentity(dname);
                              id.getDefaultKey().setDefaultCertificate(*cert);
                              m_onSuccess(m_node);
                            },
                            m_onFailure);
}

} // namespace nacapp