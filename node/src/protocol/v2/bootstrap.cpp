#include "bootstrap.hpp"

#include <ndn-cxx/util/string-helper.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "../../../../shared/src/buffer-helper.hpp"
#include "../../../../shared/src/hmac.hpp"
#include "../../../../shared/src/ndn-util.hpp"
#include "../../../../shared/src/string_helpers.hpp"

#include "bootstrap-impl.hpp"


namespace nacapp {
namespace bootstrap {

bool
isValidSession(std::shared_ptr<Session> session)
{
  bool isNotNull = session != nullptr;
  bool keychainExist = session->keychain != nullptr;

  return isNotNull && keychainExist;
}

bool
isValidConfig(const Config& cfg)
{
  bool pairIdIsValid = cfg.pairId.size() > 0;
  bool pairCodeIsValid = cfg.pairCode.size() > 0;
  return pairIdIsValid && pairCodeIsValid;
}

void
start(std::shared_ptr<Node> node,
      std::shared_ptr<Session> session,
      const Config& cfg,
      OnSuccess onSuccess,
      OnFail onFail)
{
  if (!isValidSession(session)) {
    std::string msg = "invalid initial session";
    LOG(ERROR) << msg;
    onFail(msg);
    return;
  }

  if (!isValidConfig(cfg)) {
    std::string msg = "invalid bootstrap config";
    LOG(ERROR) << msg;
    onFail(msg);
    return;
  }

  impl::start(node,
              session,
              cfg,
              [&](std::shared_ptr<Certificate> owner) {
                session->ownerCert = owner;
                impl::setTrustAnchor(session->validator, node, owner);
              },
              [&](std::shared_ptr<Certificate> signedDeviceCert) {
                impl::setDeviceCert(session->keychain, session->deviceIdentity, signedDeviceCert);
                onSuccess();
              },
              onFail);
}

} // namespace bootstrap
} // namespace nacapp