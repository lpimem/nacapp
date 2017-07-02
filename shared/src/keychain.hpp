#ifndef NACAPP_KEYCHAIN_HPP
#define NACAPP_KEYCHAIN_HPP

#include "common-cxx.hpp"
#include "common-nac.hpp"
#include "common-ndn.hpp"
#include <ndn-cxx/security/key-params.hpp>

namespace nacapp {

// extern KeyChain AppKeyChain;

extern std::shared_ptr<KeyChain>
createMemoryKeyChain();

extern const ndn::KeyParams&
getDefaultKeyParams();
}

#endif /* NACAPP_KEYCHAIN_HPP */
