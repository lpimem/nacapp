#ifndef NODE_BOOTSTRAP_V2_IMPL_HPP
#define NODE_BOOTSTRAP_V2_IMPL_HPP

#include "../../../../shared/src/common-ndn.hpp"

#include "bootstrap-helper.hpp"

namespace nacapp {
namespace bootstrap {

using OnOwnerCert = std::function<void(std::shared_ptr<Certificate>)>;
using OnDeviceCertSigned = std::function<void(const ndn::Name&, std::shared_ptr<Certificate>)>;

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
                OnStatusChange onFailure);

void
serveDeviceUnsignedCert(std::shared_ptr<Node> node,
                        const Name& wellknown,
                        const std::string& deviceId,
                        const std::string& sharedSecret,
                        const Name& dname,
                        std::shared_ptr<Certificate> deviceCertUnsigned,
                        OnDeviceCertSigned onDeviceCertSigned,
                        OnStatusChange onFailure);

void
startBootstrap(const Name& wellknown,
               const std::string& deviceId,
               const std::string& pin,
               std::shared_ptr<ndn::KeyChain> keychain,
               std::shared_ptr<Node> node,
               OnOwnerCert onOwnerCert,
               OnDeviceCertSigned onDeviceCertSigned,
               OnStatusChange onFailure);

} // namespace bootstrap
} // namespace nacapp
#endif