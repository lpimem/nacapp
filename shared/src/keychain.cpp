#include "keychain.hpp"

namespace nacapp {

// KeyChain AppKeyChain;

std::shared_ptr<KeyChain>
createMemoryKeyChain()
{
  return make_shared<KeyChain>("pib-memory:", "tpm-memory:", true);
}

typedef ndn::SimplePublicKeyParams<ndn::RsaKeyParamsInfo> RsaKeyParams;

const ndn::KeyParams&
getDefaultKeyParams()
{
  static RsaKeyParams keyParams(DEFAULT_RSA_KEY_SIZE, ndn::KeyIdType::SHA256);
  return keyParams;
}

} // nacapp