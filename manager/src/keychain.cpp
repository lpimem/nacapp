#include "keychain.hpp"

namespace nacapp {

KeyChain AppKeyChain;

typedef ndn::SimplePublicKeyParams<ndn::RsaKeyParamsInfo> RsaKeyParams;

const ndn::KeyParams &getDefaultKeyParams() {
  static RsaKeyParams keyParams;
  return keyParams;
}

} // nacapp