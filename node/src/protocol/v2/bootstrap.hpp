#ifndef NODE_BOOTSTRAP_V2_HPP
#define NODE_BOOTSTRAP_V2_HPP

#include <ndn-cxx/security/v2/validator.hpp>

#include "../../../../shared/src/common-ndn.hpp"
#include "../../node.hpp"

namespace nacapp {
namespace bootstrap {

enum class BTStatus { STARTED, SERVING, TERMINATED, SUCCESS, FAIL };

using OnSuccess = std::function<void()>;
using OnFail = std::function<void(const std::string& reason)>;

struct Session
{
  std::shared_ptr<KeyChain> keychain;
  std::shared_ptr<Identity> deviceIdentity;
  std::shared_ptr<Certificate> ownerCert;
  std::shared_ptr<ndn::security::v2::Validator> validator;
  std::string error;
  BTStatus status;
};

struct Config
{
  std::string pairId;
  std::string pairCode;
  Name wellknown;
};

void
start(std::shared_ptr<Node> node, std::shared_ptr<Session>, const Config&, OnSuccess, OnFail);

} // namespace bootstrap
} // namespace nacapp

#endif