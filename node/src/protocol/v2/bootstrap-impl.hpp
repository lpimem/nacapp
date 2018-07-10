#ifndef NODE_BOOTSTRAP_V2_IMPL_HPP
#define NODE_BOOTSTRAP_V2_IMPL_HPP

#include "../../../../shared/src/common-ndn.hpp"

#include "bootstrap-helper.hpp"

namespace nacapp {
namespace bootstrap {

bool
verifyHash(std::string content, std::string key, std::string hash);

bool
validateOwnerCertResp(const Data& d, std::string expected_r, std::string pin);

bool
validateOwnerRequest(const Interest& interest, const Name& args, std::string pin);

void
serveDeviceUnsignedCert(std::shared_ptr<Node> node,
                        const std::string& deviceId,
                        const std::string& pin,
                        std::shared_ptr<Certificate> deviceCertUnsigned,
                        OnStatusChange onSuccess,
                        OnStatusChange onFailure);

void
startBootstrap(const Name& ownerName,
               const std::string& deviceId,
               const std::string& pin,
               std::shared_ptr<Certificate> deviceCertUnsigned,
               std::shared_ptr<Node> node,
               OnStatusChange onSuccess,
               OnStatusChange onFailure);

} // namespace bootstrap
} // namespace nacapp
#endif