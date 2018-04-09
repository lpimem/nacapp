#include "keychain.hpp"

namespace nacapp {

// KeyChain AppKeyChain;

std::shared_ptr<KeyChain>
createMemoryKeyChain()
{
  return make_shared<KeyChain>("pib-memory:", "tpm-memory:", true);
}

const ndn::KeyParams&
getDefaultKeyParams()
{
  static ndn::RsaKeyParams keyParams(DEFAULT_RSA_KEY_SIZE, ndn::KeyIdType::SHA256);
  return keyParams;
}

} // namespace nacapp