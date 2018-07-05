#ifndef NACAPP_KEYCHAIN_HPP
#define NACAPP_KEYCHAIN_HPP

#include "common-cxx.hpp"
#include "common-ndn.hpp"
#include <ndn-cxx/security/key-params.hpp>

namespace nacapp {

static const uint32_t DEFAULT_RSA_KEY_SIZE = 2048;

// extern KeyChain AppKeyChain;

extern std::shared_ptr<KeyChain>
createMemoryKeyChain();

extern const ndn::KeyParams&
getDefaultKeyParams();
} // namespace nacapp

#endif /* NACAPP_KEYCHAIN_HPP */
