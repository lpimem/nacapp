#ifndef NACAPP_KEYCHAIN_HPP
#define NACAPP_KEYCHAIN_HPP

#include "common.hpp"
#include "ndn-cxx/security/key-params.hpp"

namespace nacapp
{

extern KeyChain AppKeyChain;

extern const ndn::KeyParams &getDefaultKeyParams();
}

#endif /* NACAPP_KEYCHAIN_HPP */
