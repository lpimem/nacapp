#include "keychain.hpp"
#include "../../shared/src/easylogging/easylogging++.h"

namespace nacapp
{

// KeyChain AppKeyChain{"pib-memory", "tpm-memory"};
KeyChain AppKeyChain;

typedef ndn::SimplePublicKeyParams<ndn::RsaKeyParamsInfo> RsaKeyParams;

const ndn::KeyParams &getDefaultKeyParams()
{
  static RsaKeyParams keyParams;
  // LOG(DEBUG) << "Key Type: " << keyParams.getKeyType();
  // LOG(DEBUG) << "Key Size: " << keyParams.getKeySize();
  return keyParams;
}

} // nacapp