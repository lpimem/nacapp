#ifndef NODE_BOOTSTRAP_V2_IMPL_HPP
#define NODE_BOOTSTRAP_V2_IMPL_HPP

#include "../../../../shared/src/common-ndn.hpp"
#include "../../node.hpp"

#include "bootstrap.hpp"

namespace nacapp {
namespace bootstrap {
namespace impl {

using OnOwnerCert = std::function<void(std::shared_ptr<Certificate>)>;
using OnDeviceCertSigned = std::function<void(std::shared_ptr<Certificate>)>;

bool
verifyHash(std::string content, std::string key, std::string hash);

bool
validateOwnerCertResp(const Data& d, std::string expected_r, std::string pin);

bool
validateOwnerRequest(const Interest& interest, const Name& args, std::string pin);

void
fetchDeviceCert(std::shared_ptr<Node> node,
                const Name& wellknown,
                const std::string& deviceId,
                const Name& dname,
                OnDeviceCertSigned onDeviceCertSigned,
                OnFail onFailure);

void
serveDeviceUnsignedCert(std::shared_ptr<Node> node,
                        const Name& wellknown,
                        const std::string& deviceId,
                        const std::string& sharedSecret,
                        const Name& dname,
                        std::shared_ptr<Certificate> deviceCertUnsigned,
                        OnDeviceCertSigned onDeviceCertSigned,
                        OnFail onFailure);

void
startBootstrap(const Name& wellknown,
               const std::string& deviceId,
               const std::string& pin,
               std::shared_ptr<ndn::KeyChain> keychain,
               std::shared_ptr<Node> node,
               OnOwnerCert onOwnerCert,
               OnDeviceCertSigned onDeviceCertSigned,
               OnFail onFailure);

void
start(std::shared_ptr<Node> node,
      std::shared_ptr<Session> session,
      const Config& cfg,
      OnOwnerCert onOwnerCert,
      OnDeviceCertSigned onDeviceCertSigned,
      OnFail onFail);

void
setTrustAnchor(std::shared_ptr<ndn::security::v2::Validator> validator,
               std::shared_ptr<Node> node,
               std::shared_ptr<Certificate> anchor);


void
setDeviceCert(std::shared_ptr<KeyChain> keychain,
              std::shared_ptr<Identity> id,
              std::shared_ptr<Certificate> cert);
} // namespace impl
} // namespace bootstrap
} // namespace nacapp
#endif