#ifndef NODE_BOOTSTRAP_V2_IMPL_HPP
#define NODE_BOOTSTRAP_V2_IMPL_HPP

#include "../../../../shared/src/common-ndn.hpp"

#include "bootstrap.hpp"

namespace nacapp {
namespace bootstrap {

bool
verifyHash(std::string content, std::string key, std::string hash);

bool
validateOwnerCertResp(const Data& d, std::string expected_r, std::string pin);

bool
validateOwnerRequest(const Interest& interest, const Name& args, std::string pin);

void
serveDeviceUnsignedCert(std::shared_ptr<BootstrapHelper> helper,
                        OnStatusChange onSuccess,
                        OnStatusChange onFailure);

void
startBootstrap(Name ownerName,
               std::string deviceID,
               std::shared_ptr<BootstrapHelper> helper,
               OnStatusChange onSuccess,
               OnStatusChange onFailure);

} // namespace bootstrap
} // namespace nacapp
#endif