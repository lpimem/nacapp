#include "keychain.hpp"

namespace nacapp {

KeyChain AppKeyChain;

typedef ndn::SimplePublicKeyParams<ndn::RsaKeyParamsInfo> RsaKeyParams;

const ndn::KeyParams&
getDefaultKeyParams()
{
  static RsaKeyParams keyParams(DEFAULT_RSA_KEY_SIZE, ndn::KeyIdType::SHA256);
  return keyParams;
}

} // nacapp